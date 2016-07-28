
#include "Internal.h"
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/Shader.h>
#include "RendererImpl.h"
#include "Text/TextRenderer.h"
#include "GeometryRenderer.h"
#include "FrameRectRenderer.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// DrawingContext
//==============================================================================

//------------------------------------------------------------------------------
DrawingContext::DrawingContext()
	: m_geometryRenderer(nullptr)
	, m_textRenderer(nullptr)
	, m_frameRectRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_frameRectRenderer);
	LN_SAFE_RELEASE(m_textRenderer);
	LN_SAFE_RELEASE(m_geometryRenderer);
}

//------------------------------------------------------------------------------
void DrawingContext::Initialize(GraphicsManager* manager)
{
	detail::ContextInterface::Initialize(manager);

	m_geometryRenderer = LN_NEW detail::GeometryRenderer();
	m_geometryRenderer->Initialize(manager);

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);

	m_frameRectRenderer = LN_NEW detail::FrameRectRenderer();
	m_frameRectRenderer->Initialize(manager);
}

//------------------------------------------------------------------------------
void DrawingContext::Set2DRenderingMode(float minZ, float maxZ)
{
	NorityStateChanging();
	const Size& size = m_state.GetRenderTarget(0)->GetSize();
	m_state.viewTransform = Matrix::Identity;
	m_state.projectionTransform = Matrix::MakePerspective2DLH((float)size.width, (float)size.height, minZ, maxZ);
}

//------------------------------------------------------------------------------
void DrawingContext::SetViewProjectionTransform(const Matrix& view, const Matrix& proj)
{
	NorityStateChanging();
	m_state.viewTransform = view;
	m_state.projectionTransform = proj;
}
//------------------------------------------------------------------------------
void DrawingContext::SetRenderTarget(int index, Texture* texture)
{
	if (texture != m_state.GetRenderTarget(index))
	{
		NorityStateChanging();
		m_state.SetRenderTarget(index, texture);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetDepthBuffer(Texture* depthBuffer)
{
	if (m_state.depthBuffer != depthBuffer)
	{
		NorityStateChanging();
		m_state.depthBuffer = depthBuffer;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	NorityStateChanging();
	m_state.worldTransform = matrix;
}

//------------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	if (m_state.opacity != opacity)
	{
		NorityStateChanging();
		m_state.opacity = opacity;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	if (m_state.fillBrush != brush)
	{
		NorityStateChanging();
		m_state.fillBrush = brush;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	if (m_state.font != font)
	{
		NorityStateChanging();
		m_state.font = font;
	}
}

//------------------------------------------------------------------------------
void DrawingContext::PushState()
{
	m_stateStack.push(m_state);
}

//------------------------------------------------------------------------------
void DrawingContext::PopState()
{
	if (!m_state.Equals(m_stateStack.top()))
	{
		NorityStateChanging();
		m_state = m_stateStack.top();
	}
	m_stateStack.pop();
}






//------------------------------------------------------------------------------
void DrawingContext::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	NorityStartDrawing(GetBaseRenderer());
	GetBaseRenderer()->Clear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void DrawingContext::MoveTo(const Vector3& point, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->MoveTo(point, color);
}

//------------------------------------------------------------------------------
void DrawingContext::LineTo(const Vector3& point, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->LineTo(point, color);
}

//------------------------------------------------------------------------------
void DrawingContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->BezierCurveTo(cp1, cp2, endPt, color);
}

//------------------------------------------------------------------------------
void DrawingContext::ClosePath()
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->ClosePath();
}

//------------------------------------------------------------------------------
//void DrawingContext::DrawPoint(const Vector3& point, const Color& color)
//{
//	NorityStartDrawing(m_geometryRenderer);
//	m_geometryRenderer->DrawPoint(point, color);
//}

//------------------------------------------------------------------------------
void DrawingContext::DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect)
{
	NorityStartDrawing(m_geometryRenderer);

	if (m_state.fillBrush->GetType() == BrushType_FrameTexture)
	{
		m_frameRectRenderer->Draw(rect);
	}
	else
	{
		m_geometryRenderer->DrawRectangle(rect);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawEllipse(center, radius);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const Color& color)
{
	NorityStartDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTexture(rect, texture, srcRect, color);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawText(const StringRef& text, const PointF& position)
{
	NorityStartDrawing(m_textRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), position);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags)
{
	NorityStartDrawing(m_textRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), rect, flags);
}




//------------------------------------------------------------------------------
void DrawingContext::Flush()
{
	detail::ContextInterface::FlushImplemented();
}

//------------------------------------------------------------------------------
void DrawingContext::OnStateFlush()
{
	ContextInterface::OnStateFlush();
	SetBasicContextState(m_state);

	m_geometryRenderer->SetTransform(m_state.worldTransform);
	m_textRenderer->SetTransform(m_state.worldTransform);

	m_geometryRenderer->SetOpacity(m_state.opacity);

	m_geometryRenderer->SetBrush(m_state.fillBrush);

	m_textRenderer->SetFont(m_state.font);






	const Size& size = m_state.GetRenderTarget(0)->GetSize();
	Matrix viewProj = m_state.viewTransform * m_state.projectionTransform;

	m_geometryRenderer->SetViewProjection(m_state.viewTransform, m_state.projectionTransform, size);
	//m_spriteRenderer->SetViewPixelSize(size);
	m_textRenderer->SetViewProjMatrix(viewProj);
	m_textRenderer->SetViewPixelSize(size);

	if (m_state.fillBrush != nullptr &&
		m_state.fillBrush->GetType() == BrushType_FrameTexture)
	{
		m_frameRectRenderer->SetState(static_cast<FrameTextureBrush*>(m_state.fillBrush.Get()), m_state.worldTransform, viewProj);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::OnPrimitiveFlush()
{
	ContextInterface::OnPrimitiveFlush();
}

//------------------------------------------------------------------------------
void DrawingContext::OnShaderVariableModified(ShaderVariable* var)
{
	if (m_state.GetShaderPass() != nullptr && m_state.GetShaderPass()->GetOwnerShader() == var->GetOwnerShader())
	{
		NorityStateChanging();
	}
}

//==============================================================================
// DrawingContext::ContextState
//==============================================================================

//------------------------------------------------------------------------------
bool DrawingContext::ContextState::Equals(const ContextState& s) const
{
	if (!BasicContextState::Equals(s)) return false;
	if (worldTransform != s.worldTransform) return false;
	if (viewTransform != s.viewTransform) return false;
	if (projectionTransform != s.projectionTransform) return false;
	if (opacity != s.opacity) return false;
	if (fillBrush != s.fillBrush) return false;
	if (font != s.font) return false;
	return true;
}


LN_NAMESPACE_END