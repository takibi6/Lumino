﻿
#include <LuminoEngine.h>
#include <Lumino/Scene/MeshModelObject.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Foundation/Application.h>
#include <Lumino/Foundation/GameScene.h>
#include <Lumino/UI/UIFrameWindow.h>
using namespace ln;







#if 0



template<class T, typename... TArgs>
RefPtr<T> NewObject(TArgs... args)
{
	auto ptr = RefPtr<T>::MakeRef();
	//ptr->Initialize(args);
	return ptr;
}


class Simple2DCharacterController
	: public Component
{
public:

	virtual void OnUpdate() override
	{
		auto player = GetOwnerObject();
		
		if (Input::IsPressed(InputButtons::Right))
			player->transform.Translate(1, 0);
	}
};


class TestScene1
	: public GameScene
{
public:

	virtual void OnStart() override
	{
		auto player = GameObject::Create();
		player->AddComponent(Sprite2D::Create(_T("C:/LocalProj/設計ツール/dll_48x48.png")));
		player->AddComponent(NewObject<Simple2DCharacterController>());
	}

	virtual void OnUpdate() override
	{
		GameScene::OnUpdate();
	}

	virtual void OnTerminate() override
	{
	}

};

void Main()
{
	GameApplication app;
	app.Run(NewObject<TestScene1>());
}

#else


void Main()
{
	EngineSettings::SetGraphicsAPI(GraphicsAPI::OpenGL);
	EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType::Immediate);//GraphicsRenderingType::Threaded);//
	Engine::Initialize();
	Engine::GetMainViewport()->SetBackgroundColor(Color32::Gray);
	Engine::GetMainViewport()->SetPlacement(ViewportPlacement::AutoResize);

	//Engine::GetDefaultSceneGraph3D()->visibleGridPlane = true;

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);


	//auto mesh = SkinnedMesh::Create(_T("D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));

	//auto tex = Texture2D::Create("D:/Proj/Volkoff/External/Lumino/test/UnitTest/Scene/TestData/Dice1.png");
	//auto sprite1 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite2 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite3 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite4 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite5 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite6 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);

	//auto mesh = StaticMesh::CreateSphere(2, 4, 4);

#if 0
	auto blur = ScreenMotionBlurImageEffect::Create();
	//blur->SetAmount(0.5f);
	//blur->SetRadialScale(1.05f);
	CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(blur);

	//auto tonePE23 = ToneImageEffect::Create();
	//tonePE23->SetTone(ToneF(1, 0,0, 1.0));
	//CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE23);

	//auto tonePE2 = ToneImageEffect::Create();
	//tonePE2->SetTone(ToneF(0,0,1, 1.0));
	//CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE2);

	auto tonePE = ToneImageEffect::Create();
	//tonePE->SetTone(ToneF(0,0,1, 1.0));
	tonePE->ChangeTone(ToneF(1, 0, 1, 0), 5);
	CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE);
#endif

#if 0
	//Engine::GetMainLight3D()->SetPosition();
	auto mLogoSprite = Sprite2D::Create(_T("D:/Proj/Volkoff/Volkoff/Data/Graphics/Frontend/Logo_1.png"));

	int map[5*5] =
	{
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
	};

	auto t = Assets::LoadTexture(_T("D:/Proj/Volkoff/Volkoff/Data/Graphics/MapChip_1.png"));

	auto m_tileset = TileSet::Create();
	m_tileset->SetImageSource(t);
	m_tileset->SetTileSize(SizeI(20, 20));


	auto tilemapModel = TileMapModel::Create();
	tilemapModel->SetTileSet(m_tileset);

	auto m_tileLayer = TileLayer::Create();
	m_tileLayer->Resize(5, 5);
	tilemapModel->GetLayers()->Add(m_tileLayer);

	auto mTilemap = TileMap::Create3D();
	mTilemap->SetTileMap(tilemapModel);
	mTilemap->SetPriority(1000);
	mTilemap->SetDepthWriteEnabled(false);

	for (int i = 0; i < 5 * 5; ++i)
	{
		m_tileLayer->SetTileId(i % 100, i / 100, map[i]);
	}
#endif

	EngineDiag::SetDisplayMode(EngineDiagDisplayMode::FpsSummary);

	Input::AddButtonBinding(_T("AA"), KeyboardBinding::Create(Keys::C));
	Input::AddButtonBinding(_T("GG"), KeyboardBinding::Create(Keys::A));

	//auto spr1 = Sprite2D::Create(_T("D:/Proj/LuminoStudio/Engine/External/Lumino/test/UnitTest/Graphics/TestData/Sprite2.png"));

	auto text = TextBlock2D::Create(_T("Hello, world!"));

	while (Engine::Update())
	{
		//printf("----\n");
		if (Input::IsTriggered(InputButtons::Cancel))
		{
			//tonePE->SetTone(ToneF(-1, -1, -1, 0));
			//tonePE->ChangeTone(ToneF(1, 1, 1, 0), 0.5);
			//blur->SetBlurStatus(0.5, Vector2::Zero, 1.05, 0.5);

			Engine::GetMainWindow()->SetSize(SizeI(200, 100));
		}
		//blur->SetBlurStatus(0.9f, Vector2::Zero, 1.02);
	}
}

#endif
