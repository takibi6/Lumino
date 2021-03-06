﻿
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Animation/AnimationManager.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Platform/PlatformSupport.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/RenderingThread.h"
#include "Graphics/GraphicsManager.h"
#include "Documents/DocumentsManager.h"
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include "Effect/EffectManager.h"
#include "UI/UIManager.h"
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/Foundation/Application.h>
#include "AssetsManager.h"
#include "EngineDiagViewer.h"
#include "EngineManager.h"
#include "Resource.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// EngineSettings
//==============================================================================

detail::EngineSettings detail::EngineSettings::instance;

//------------------------------------------------------------------------------
void EngineSettings::SetMainWindowSize(const SizeI& size)
{
	detail::EngineSettings::instance.mainWindowSize = size;
}
void EngineSettings::SetMainWindowSize(int width, int height)
{
	detail::EngineSettings::instance.mainWindowSize.Set(width, height);
}

//------------------------------------------------------------------------------
void EngineSettings::SetMainBackBufferSize(const SizeI& size)
{
	detail::EngineSettings::instance.mainBackBufferSize = size;
}
void EngineSettings::SetMainBackBufferSize(int width, int height)
{
	detail::EngineSettings::instance.mainBackBufferSize.Set(width, height);
}

//------------------------------------------------------------------------------
void EngineSettings::SetMainWindowTitle(const StringRef& title)
{
	detail::EngineSettings::instance.mainWindowTitle = title;
}

//------------------------------------------------------------------------------
void EngineSettings::SetEngineLogEnabled(bool enabled)
{
	detail::EngineSettings::instance.engineLogEnabled = enabled;
}

//------------------------------------------------------------------------------
void EngineSettings::SetGraphicsAPI(GraphicsAPI graphicsAPI)
{
	detail::EngineSettings::instance.graphicsAPI = graphicsAPI;
}

//------------------------------------------------------------------------------
void EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType renderingType)
{
	detail::EngineSettings::instance.renderingType = renderingType;
}

//------------------------------------------------------------------------------
void EngineSettings::SetFpuPreserveEnabled(bool enabled)
{
	detail::EngineSettings::instance.fpuPreserveEnabled = enabled;
}

//------------------------------------------------------------------------------
void EngineSettings::SetDirectMusicMode(DirectMusicMode mode)
{
	detail::EngineSettings::instance.directMusicMode = mode;
}

//------------------------------------------------------------------------------
void EngineSettings::SetDirectMusicReverbLevel(float level)
{
	detail::EngineSettings::instance.DirectMusicReverbLevel = level;
}

//==============================================================================
// EngineManager
//==============================================================================

EngineManager* EngineManager::Instance = nullptr;
const TCHAR* EngineManager::LogFileName = _T("EngineLog.txt");

//------------------------------------------------------------------------------
EngineManager* EngineManager::GetInstance(EngineManager* priority)
{
	if (priority != nullptr)
		return priority;
	return EngineManager::Instance;
}

//------------------------------------------------------------------------------
EngineManager* EngineManager::Create(const detail::EngineSettings& configData)
{
	RefPtr<EngineManager> app(LN_NEW EngineManager(configData), false);
	app.SafeAddRef();
	return app;
}

//------------------------------------------------------------------------------
EngineManager::EngineManager(const detail::EngineSettings& configData)
	: m_configData(configData)
	, m_animationManager(nullptr)
	, m_fileManager(nullptr)
	, m_inputManager(nullptr)
	, m_audioManager(nullptr)
	, m_physicsManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_effectManager(nullptr)
	, m_modelManager(nullptr)
	, m_documentsManager(nullptr)
	, m_uiManager(nullptr)
	, m_sceneGraphManager(nullptr)
	, m_assetsManager(nullptr)
	, m_diagViewer(nullptr)
	, m_fixedDeltaTime(0.0f)
	, m_frameRenderingSkip(false)
	, m_frameRenderd(false)
	, m_commonInitied(false)
	, m_endRequested(false)
	, m_comInitialized(false)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);


#if defined(LN_OS_WIN32)
	// COM 初期化
	if (m_configData.autoCoInitialize && SUCCEEDED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		// エラーにはしない。別の設定で COM が初期化済みだったりすると失敗することがあるが、COM 自体は使えるようになっている
		m_comInitialized = true;
	}
#endif
}

//------------------------------------------------------------------------------
EngineManager::~EngineManager()
{
	LN_SAFE_RELEASE(m_diagViewer);

	if (m_assetsManager != nullptr)
	{
		m_assetsManager->Finalize();
		LN_SAFE_RELEASE(m_assetsManager);
	}

	if (m_graphicsManager != nullptr)
	{
		// 先に描画スレッドを終了しておく。
		// 他モジュールで発行されたコマンドがまだ実行待機中にそのモジュールが解放されるとマズイ。
		m_graphicsManager->Finalize();
	}

	if (m_platformManager != nullptr)
	{
		m_platformManager->GetMainWindow()->DetachEventListener(this);
		m_platformManager->Dispose();
	}
	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->ReleaseDefaultSceneGraph();
		LN_SAFE_RELEASE(m_sceneGraphManager);
	}
	if (m_modelManager != nullptr) {
		m_modelManager->Finalize();
		LN_SAFE_RELEASE(m_modelManager);
	}
	if (m_effectManager != nullptr)
	{
		m_graphicsManager->RemoveDeviceResetListener(m_effectManager);
		m_effectManager->Finalize();
		LN_SAFE_RELEASE(m_effectManager);
	}
	if (m_uiManager != nullptr) {
		m_uiManager->Finalize();
		LN_SAFE_RELEASE(m_uiManager);
	}

	LN_SAFE_RELEASE(m_documentsManager);

	LN_SAFE_RELEASE(m_graphicsManager);

	if (m_physicsManager != nullptr) {
		m_physicsManager->Finalize();
		m_physicsManager.SafeRelease();
		//LN_SAFE_RELEASE(m_physicsManager);
	}
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager != nullptr) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}
#endif
	if (m_inputManager != nullptr) {
		m_inputManager->Finalize();
		LN_SAFE_RELEASE(m_inputManager);
	}

	LN_SAFE_RELEASE(m_fileManager);

	LN_SAFE_RELEASE(m_animationManager);


#if defined(LN_OS_WIN32)
	if (m_comInitialized)
	{
		::CoUninitialize();
		m_comInitialized = false;
	}
#endif
}

//------------------------------------------------------------------------------
void EngineManager::Initialize()
{
	InitializePlatformManager();
	InitializeInputManager();
	InitializeAudioManager();
	InitializePhysicsManager();
	InitializeGraphicsManager();
	InitializeEffectManager();
	InitializeModelManager();
	InitializeUIManager();
#ifdef LN_BUILD_SCENE_MODULE
	InitializeSceneGraphManager();
#endif
	InitializeAssetsManager();

	EngineDiagCore::Instance.Initialize(this);

	// デフォルトで作成されるレイヤーのサイズを更新しておく
	// TODO: レイヤーを最初に作ったときでもいいか？
	if (m_uiManager != nullptr)
	{
		m_uiManager->GetMainWindow()->UpdateViewportTransform();
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeCommon()
{
	if (!m_commonInitied)
	{
		InternalResource::InitializeEngineResource();

		// ログファイル出力
		if (m_configData.engineLogEnabled) {
			Logger::Initialize(LogFileName);
		}
		m_commonInitied = true;
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeAnimationManager()
{
	if (m_animationManager == nullptr)
	{
		m_animationManager = LN_NEW detail::AnimationManager();
		m_animationManager->Initialize();
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeFileManager()
{
	if (m_fileManager == nullptr)
	{
		FileManager::Settings data;
		data.AccessMode = m_configData.FileAccessPriority;
		m_fileManager = LN_NEW FileManager(data);
		for (auto& e : m_configData.ArchiveFileEntryList) {
			m_fileManager->RegisterArchive(e.filePath, e.password);
		}
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		InitializeCommon();

		PlatformManager::Settings data;
		data.windowSystemAPI = m_configData.windowSystemAPI;
		data.mainWindowSettings.title = m_configData.mainWindowTitle;
		data.mainWindowSettings.clientSize = m_configData.mainWindowSize;
		data.mainWindowSettings.fullscreen = false;
		data.mainWindowSettings.resizable = true;
		data.mainWindowSettings.userWindow = m_configData.userMainWindow;
		data.useInternalUIThread = false;

		m_platformManager.Attach(LN_NEW PlatformManager());
		m_platformManager->Initialize(data);

		// イベントリスナー登録
		m_platformManager->GetMainWindow()->AttachEventListener(this, 0);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeInputManager()
{
	if (m_inputManager == nullptr)
	{
		InitializePlatformManager();

		detail::InputManager::Settings data;
		data.mainWindow = m_platformManager->GetMainWindow();

		m_inputManager = LN_NEW detail::InputManager();
		m_inputManager->Initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeAudioManager()
{
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();

		// ユーザー定義のウィンドウハンドルが指定されている場合、
		// ダミーウィンドウクラスを作るために PlatformManager の初期化が必要。
		if (m_configData.userMainWindow != nullptr) {
			InitializePlatformManager();
		}

		detail::AudioManager::Settings settings;
		settings.fileManager = m_fileManager;
		settings.streamCacheObjectCount = m_configData.soundCacheCapacity.objectCount;
		settings.streamSourceCacheMemorySize = m_configData.soundCacheCapacity.memorySize;
		settings.directMusicInitMode = m_configData.directMusicMode;
#ifdef LN_OS_WIN32
		settings.hWnd = (m_platformManager != nullptr) ? PlatformSupport::GetWindowHandle(m_platformManager->GetMainWindow()) : nullptr;
#endif
		settings.directMusicReverbLevel = m_configData.DirectMusicReverbLevel;
		m_audioManager = LN_NEW detail::AudioManager();
		m_audioManager->Initialize(settings);
	}
#endif
}

//------------------------------------------------------------------------------
void EngineManager::InitializePhysicsManager()
{
	if (m_physicsManager.IsNull())
	{
		InitializeCommon();
		m_physicsManager = RefPtr<detail::PhysicsManager>::MakeRef();
		m_physicsManager->Initialize();
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeGraphicsManager()
{
	if (m_graphicsManager == nullptr)
	{
		InitializeCommon();
		InitializeAnimationManager();
		InitializeFileManager();
		InitializePlatformManager();
		InitializePhysicsManager();

		detail::GraphicsManager::ConfigData data;
		data.graphicsAPI = m_configData.graphicsAPI;
		data.renderingType = m_configData.renderingType;
		data.mainWindow = m_platformManager->GetMainWindow();
		data.backBufferSize = m_configData.mainBackBufferSize;
		data.animationManager = m_animationManager;
		data.fileManager = m_fileManager;
		data.physicsManager = m_physicsManager;
		data.platformTextureLoading = true;
		data.diag = &EngineDiagCore::Instance;
#ifdef LN_OS_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
		m_graphicsManager = LN_NEW detail::GraphicsManager();
		m_graphicsManager->Initialize(data);

		m_diagViewer = LN_NEW EngineDiagViewer();
		m_diagViewer->Initialize(this, &EngineDiagCore::Instance);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeEffectManager()
{
	if (m_effectManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();

		detail::EffectManager::Settings data;
		data.fileManager = m_fileManager;
		data.audioManager = m_audioManager;
		data.graphicsManager = m_graphicsManager;
		m_effectManager = LN_NEW detail::EffectManager();
		m_effectManager->Initialize(data);

		m_graphicsManager->AddDeviceResetListener(m_effectManager);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeModelManager()
{
	if (m_modelManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();
		InitializePhysicsManager();

		detail::ModelManager::ConfigData data;
		data.fileManager = m_fileManager;
		data.physicsManager = m_physicsManager;
		data.graphicsManager = m_graphicsManager;
		data.modelCoreCacheSize = 32;
		data.modelCoreCacheMemorySize = 0;
		m_modelManager = LN_NEW detail::ModelManager();
		m_modelManager->Initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeDocumentsManager()
{
	if (m_documentsManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();

		detail::DocumentsManager::ConfigData data;
		data.graphicsManager = m_graphicsManager;
		m_documentsManager = LN_NEW detail::DocumentsManager();
		m_documentsManager->Initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::InitializeUIManager()
{
	if (m_uiManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();
		InitializeAnimationManager();
		InitializePlatformManager();
		InitializeGraphicsManager();
		InitializeDocumentsManager();
		InitializeAssetsManager();

		detail::UIManager::Settings data;
		data.fileManager = m_fileManager;
		data.animationManager = m_animationManager;
		data.platformManager = m_platformManager;
		data.graphicsManager = m_graphicsManager;
		data.assetsManager = m_assetsManager;
		data.mainWindow = m_platformManager->GetMainWindow();
		data.documentsManager = m_documentsManager;
		data.defaultSkinFilePath = m_configData.defaultSkinFilePath;
		m_uiManager = LN_NEW detail::UIManager();
		m_uiManager->Initialize(data);
	}
}

//------------------------------------------------------------------------------
#ifdef LN_BUILD_SCENE_MODULE
void EngineManager::InitializeSceneGraphManager()
{
	if (m_sceneGraphManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();
		InitializeGraphicsManager();
		InitializePhysicsManager();
		InitializeEffectManager();
		InitializeModelManager();
		InitializeUIManager();
		InitializeDocumentsManager();

		SceneGraphManager::ConfigData data;
		data.engineDiag = &EngineDiagCore::Instance;
		data.FileManager = m_fileManager;
		data.GraphicsManager = m_graphicsManager;
		data.PhysicsManager = m_physicsManager;
		data.effectManager = m_effectManager;
		data.modelManager = m_modelManager;
		data.documentsManager = m_documentsManager;
		data.mainViewport = m_uiManager->GetMainWindow()->GetViewport();
		m_sceneGraphManager = LN_NEW SceneGraphManager(data);
		m_sceneGraphManager->CreateDefaultSceneGraph();
		SceneGraphManager::Instance = m_sceneGraphManager;
	}
}
#endif

//------------------------------------------------------------------------------
void EngineManager::InitializeAssetsManager()
{
	if (m_assetsManager == nullptr)
	{
		m_assetsManager = LN_NEW AssetsManager();
		m_assetsManager->Initialize(this);
	}
}

//------------------------------------------------------------------------------
//bool EngineManager::Update()
//{
//	UpdateFrame2();
//
//	// 手動描画されていなければここで自動描画する
//	if (!m_frameRenderd)
//	{
//		if (BeginRendering())
//		{
//			Render();
//			EndRendering();
//		}
//	}
//
//	m_frameRenderd = false;
//	return !m_endRequested;
//}
//
////------------------------------------------------------------------------------
//bool EngineManager::UpdateFrame2()
//{
//	return !m_endRequested;
//}

//------------------------------------------------------------------------------
bool EngineManager::UpdateUnitily()
{
	BeginFrameUpdate();
	try
	{
		if (BeginRendering())
		{
			Render();
			EndRendering();
		}
		EndFrameUpdate();
	}
	catch (...)
	{
		EndFrameUpdate();
		throw;
	}
	return !IsEndRequested();
}

//------------------------------------------------------------------------------
void EngineManager::BeginFrameUpdate()
{
	m_fpsController.Process();

	Profiler::Instance.SetMainFPS(m_fpsController.GetFps());
	Profiler::Instance.SetMainFPSCapacity(m_fpsController.GetCapacityFps());

	m_diagViewer->UpdateFrame();




	float deltaTime = m_fixedDeltaTime;
	if (deltaTime == 0.0f)
	{
		deltaTime = m_fpsController.GetElapsedGameTime();
	}

	if (m_inputManager != nullptr) {
		m_inputManager->PreUpdateFrame();
	}

	if (!m_platformManager->DoEvents())
	{
		m_endRequested = true;
	}

	if (m_animationManager != nullptr)
	{
		m_animationManager->AdvanceTime(deltaTime);
	}

	if (m_inputManager != nullptr) {
		m_inputManager->UpdateFrame();
	}

	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->UpdateFrameDefaultSceneGraph(deltaTime);
	}

	if (m_uiManager != nullptr)
	{
		m_uiManager->GetMainWindow()->UpdateViewportTransform();
		m_uiManager->GetMainWindow()->InjectElapsedTime(deltaTime);

		{	// プロファイリング範囲
			ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);
			const SizeI& size = m_graphicsManager->GetMainSwapChain()->GetBackBuffer()->GetSize();
			m_uiManager->GetMainWindow()->UpdateLayout(Size(static_cast<float>(size.width), static_cast<float>(size.height)));
		}
	}
}

//------------------------------------------------------------------------------
void EngineManager::EndFrameUpdate()
{

}

//------------------------------------------------------------------------------
bool EngineManager::BeginRendering()
{
	m_frameRenderingSkip = true;
	if (m_graphicsManager == nullptr || m_uiManager == nullptr) return nullptr;

	// 描画遅延の確認
	bool skip = false;
	if (m_graphicsManager->GetRenderingType() == GraphicsRenderingType::Threaded)
	{
		if (m_configData.delayedRenderingSkip &&
			m_graphicsManager->GetRenderingThread()->IsRunning())
		{
			skip = true;
		}
	}
	else {
		// TODO:
	}

	if (skip)
		return false;

	m_frameRenderingSkip = false;



	//if (m_effectManager != nullptr) {
	//	m_effectManager->PreRender();	// Effekseer の更新スレッドを開始するのはここ
	//	// TODO: これも UIMainWindow::BeginRendring の中かなぁ・・・
	//}

	m_uiManager->GetMainWindow()->BeginRendering();

	m_frameRenderd = true;
	return true;
}

//------------------------------------------------------------------------------
void EngineManager::EndRendering()
{
	if (m_graphicsManager == nullptr || m_frameRenderingSkip) return;

	m_uiManager->GetMainWindow()->EndRendering();
}

//------------------------------------------------------------------------------
void EngineManager::Render()
{
	if (m_graphicsManager != nullptr)
	{
		EngineDiagCore::Instance.ResetGraphicsFrameReport();	// TODO: GameMode のみ？
		EngineDiagCore::Instance.ResetVisualNodeDrawCount();	// TODO: GameMode のみ？
		m_uiManager->GetMainWindow()->RenderContents();
		//
		//if (m_uiManager != nullptr) {
		//	g->Clear(ClearFlags::Depth, ColorF::White);	// TODO
		//	g->Set2DRenderingMode(-1, 1);	// TODO
		//	m_uiManager->GetMainWindow()->RenderUI();
		//}

		if (m_diagViewer != nullptr)
		{
			//m_diagViewer->Render(g, Vector2(640, 480));	//TODO
		}

		//g->PopState();
	}
}

//------------------------------------------------------------------------------
void EngineManager::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//------------------------------------------------------------------------------
void EngineManager::Exit()
{
	m_endRequested = true;
}

//------------------------------------------------------------------------------
detail::PhysicsManager* EngineManager::GetPhysicsManager() const
{
	return m_physicsManager;
}

//------------------------------------------------------------------------------
bool EngineManager::OnEvent(const PlatformEventArgs& e)
{
	UILayoutView* uiView = nullptr;
	if (m_uiManager != nullptr)
	{
		uiView = m_uiManager->GetMainWindow()->GetMainUIContext()->GetMainWindowView();
	}


	switch (e.type)
	{
	case PlatformEventType::Quit:	// アプリ終了要求
	case PlatformEventType::Close:	// ウィンドウが閉じられようとしている
		break;

	case PlatformEventType::MouseDown:		// マウスボタンが押された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseButtonDown(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefaultSceneGraph3D()->InjectMouseButtonDown(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseUp:			// マウスボタンが離された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseButtonUp(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefaultSceneGraph3D()->InjectMouseButtonUp(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseMove:		// マウスが移動した
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseMove(e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefaultSceneGraph3D()->InjectMouseMove(e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseWheel:		// マウスホイールが操作された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseWheel(e.wheel.delta)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefaultSceneGraph3D()->InjectMouseWheel(e.wheel.delta)) { return true; }
		}
		break;
	case PlatformEventType::KeyDown:
		if (uiView != nullptr)
		{
			if (uiView->InjectKeyDown(e.key.keyCode, e.key.modifierKeys)) { return true; }
		}

		//// デバッグ表示切替
		//if (m_configData.acceleratorKeys.toggleShowDiag != nullptr &&
		//	m_configData.acceleratorKeys.toggleShowDiag->EqualKeyInput(e.key.keyCode, e.key.modifierKeys) &&
		//	m_diagViewer != nullptr)
		//{
		//	m_diagViewer->ToggleDisplayMode();
		//}
		break;
	case PlatformEventType::KeyUp:
		if (uiView != nullptr)
		{
			if (uiView->InjectKeyUp(e.key.keyCode, e.key.modifierKeys/*, e.Key.keyChar*/)) { return true; }
		}
		break;
	case PlatformEventType::KeyChar:
		if (uiView != nullptr)
		{
			if (uiView->InjectTextInput(e.key.keyChar)) { return true; }
		}
		break;
	case PlatformEventType::WindowSizeChanged:
		if (m_graphicsManager != nullptr)
		{
			m_graphicsManager->GetMainSwapChain()->Resize(SizeI(e.size.width, e.size.height));
		}
		//if (uiView != nullptr)
		//{
		//	if (uiView->InjectViewportSizeChanged(e.size.width, e.size.height)) { return true; }
		//}
		break;
	default:
		break;
	}

	if (m_inputManager != nullptr) {
		m_inputManager->OnEvent(e);
	}
	return false;
}

////------------------------------------------------------------------------------
//void EngineManager::OnLostDevice()
//{
//	if (m_effectManager != nullptr)
//	{
//		m_effectManager->OnLostDevice();
//	}
//}
//
////------------------------------------------------------------------------------
//void EngineManager::OnResetDevice()
//{
//	if (m_effectManager != nullptr)
//	{
//		m_effectManager->OnResetDevice();
//	}
//}

LN_NAMESPACE_END
