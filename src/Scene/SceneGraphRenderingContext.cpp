﻿
#include "../Internal.h"
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include "../Graphics/SpriteRenderer.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SceneGraphRenderingContext
//==============================================================================

//------------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext()
{
}

//------------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::Initialize(SceneGraphManager* manager)
{
	RenderingContext::Initialize(manager->GetGraphicsManager());

	m_spriteRenderer = RefPtr<detail::SpriteRenderer>::MakeRef(manager->GetGraphicsManager(), 2048);	// TODO
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::SetCurrentCamera(Camera* camera)
{
	if (CurrentCamera != camera)
	{
		NorityStateChanging();
		CurrentCamera = camera;
	}
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::DrawSprite2D(
	const Matrix& transform,
	const SizeF& size,
	Texture* texture,
	const RectF& srcRect,
	const Color& color)
{
	NorityStartDrawing(m_spriteRenderer);
	m_spriteRenderer->SetTransform(transform);
	m_spriteRenderer->DrawRequest2D(Vector3::Zero, Vector3::Zero, Vector2(size.width, size.height), texture, srcRect, color);
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::OnStateFlush()
{
	RenderingContext::OnStateFlush();

	const SizeI& size = GetRenderTarget(0)->GetSize();
	m_spriteRenderer->SetViewPixelSize(size);
	m_spriteRenderer->SetViewProjMatrix(CurrentCamera->GetViewMatrix(), CurrentCamera->GetProjectionMatrix());
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
