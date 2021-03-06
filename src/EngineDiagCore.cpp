﻿
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/EngineDiag.h>
#include "Graphics/GraphicsManager.h"
#include "Graphics/Text/FontManager.h"
#include "EngineManager.h"
#include "EngineDiagCore.h"
#include "EngineDiagViewer.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// EngineDiag
//==============================================================================

//------------------------------------------------------------------------------
void EngineDiag::SetDisplayMode(EngineDiagDisplayMode mode)
{
	EngineManager::GetInstance()->GetEngineDiagViewer()->SetDisplayMode(mode);
}

//------------------------------------------------------------------------------
int EngineDiag::GetGraphicsDeviceDrawCount()
{
	return EngineDiagCore::Instance.GetGraphicsDeviceDrawCount();
}

//------------------------------------------------------------------------------
int EngineDiag::GetVisualNodeDrawCount()
{
	return EngineDiagCore::Instance.GetVisualNodeDrawCount();
}

//==============================================================================
// EngineDiagCore
//==============================================================================

EngineDiagCore EngineDiagCore::Instance;

//------------------------------------------------------------------------------
EngineDiagCore::EngineDiagCore()
	: m_graphicsDeviceDrawCount(0)
{
}

//------------------------------------------------------------------------------
EngineDiagCore::~EngineDiagCore()
{
}

//------------------------------------------------------------------------------
void EngineDiagCore::Initialize(EngineManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}

//------------------------------------------------------------------------------
void EngineDiagCore::ReportCapability(int indent, const char* name, const char* value)
{
	StringA str(indent, '\t');
	str = StringA::Format("{0}{1} : {2}", str, name, value);
	Logger::WriteLine(str.c_str());
}

//------------------------------------------------------------------------------
float EngineDiagCore::GetMainFPS() const
{
	return m_manager->GetFpsController().GetFps();
}

//------------------------------------------------------------------------------
float EngineDiagCore::GetMainFPSCapacity() const
{
	return m_manager->GetFpsController().GetCapacityFps();
}

//==============================================================================
// EngineDiagViewer
//==============================================================================

//------------------------------------------------------------------------------
EngineDiagViewer::EngineDiagViewer()
	: m_diagCore(nullptr)
	, m_mainWindow(nullptr)
	, m_font(nullptr)
	, m_displayMode(EngineDiagDisplayMode::Hide)
{
#ifdef LN_DEBUG
	m_displayMode = EngineDiagDisplayMode::FpsSummary;
#endif
}

//------------------------------------------------------------------------------
EngineDiagViewer::~EngineDiagViewer()
{
}

//------------------------------------------------------------------------------
void EngineDiagViewer::Initialize(EngineManager* manager, EngineDiagCore* diagCore)
{
	LN_CHECK_ARG(manager != nullptr);
	m_diagCore = diagCore;
	m_mainWindow = manager->GetPlatformManager()->GetMainWindow();
	m_originalMainWindowTitle = m_mainWindow->GetTitleText();

	m_font = manager->GetGraphicsManager()->GetFontManager()->GetBuiltinFont(BuiltinFontSize::XXSmall);
	//m_windowRect.Set(640 - 8 - 300, 8, 300, 256);	// TODO
	m_windowRect.Set(8, 8, 300, 300);
}

//------------------------------------------------------------------------------
void EngineDiagViewer::ToggleDisplayMode()
{
	int d = (int)m_displayMode;
	d = (d + 1) % DisplayModeCycle;
	m_displayMode = (EngineDiagDisplayMode)d;
}

//------------------------------------------------------------------------------
void EngineDiagViewer::UpdateFrame()
{
	if (m_displayMode == EngineDiagDisplayMode::FpsSummary)
	{
		String str = String::Format(_T("{0} - MainFPS:{1}/{2}"), m_originalMainWindowTitle, m_diagCore->GetMainFPS(), m_diagCore->GetMainFPSCapacity());
		m_mainWindow->SetTitleText(str);
	}
}

//------------------------------------------------------------------------------
#if 0
void EngineDiagViewer::Render(DrawingContext* g, const Vector2& viewSize)
{
	if (m_displayMode != DisplayMode_Details) return;

	PointF location(m_windowRect.x, m_windowRect.y);
	g->SetOpacity(0.5f);
	g->SetFont(m_font);

	//g->SetBrush(ColorBrush::DimGray);

	// ウィンドウ背景
	g->DrawRectangle(m_windowRect);

	// キャプションバー
	g->DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.width, 20));


	g->SetBrush(ColorBrush::White);
	g->DrawText(_T("Statistics"), m_windowRect, StringFormatFlags::CenterAlignment);


	//LocalPainter painter(Size(viewSize.X, viewSize.Y), m_manager);
	////g->SetProjection(Size(viewSize.X, viewSize.Y), 0, 1000);
	//g->SetOpacity(0.5f);
	//g->SetFont(m_font);

	//// ウィンドウ背景
	//g->SetBrush(ColorBrush::DimGray);
	//g->DrawRectangle(m_windowRect);

	//// キャプションバー
	//g->SetBrush(ColorBrush::Black);
	//g->DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.Width, 20));


	//g->SetBrush(ColorBrush::White);
	//g->SetOpacity(1.0f);
	//g->DrawString(_T("Statistics"), -1, m_windowRect, StringFormatFlags::CenterAlignment);

	location.y += 24;

	//-----------------------------------------------------
	// Main info
	g->DrawText(_T("Main information:"), PointF(location.x + 8, location.y));
	location.y += 16;
	location.x += 16;

	TCHAR text[256] = { 0 };

	//StringTraits::SPrintf(text, 256, _T("Graphics API    : %s"), m_manager->GetGraphicsAPI().ToString().c_str());
	//g->DrawText(text, -1, location);
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Rendering type  : %s"), m_manager->GetRenderingType().ToString().c_str());
	//g->DrawText(text, -1, location);
	location.y += 16;

	StringTraits::SPrintf(text, 256, _T("Average FPS     : %.1f"), m_diagCore->GetMainFPS());
	g->DrawText(text, location);

	StringTraits::SPrintf(text, 256, _T(" / Capacity : %.1f"), m_diagCore->GetMainFPSCapacity());
	g->DrawText(text, PointF(location.x + 150, location.y));
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->GetCommitedMainWindowSize().Width, m_profiler->GetCommitedMainWindowSize().Height);
	//g->DrawText(text, -1, location);
	//location.Y += 16;

	//StringTraits::SPrintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->GetCommitedMainBackbufferSize().Width, m_profiler->GetCommitedMainBackbufferSize().Height);
	//g->DrawText(text, -1, location);
	//location.Y += 24;
	//location.X -= 16;


	g->Flush();
}
#endif

LN_NAMESPACE_END
