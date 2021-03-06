﻿/*
	[2015/12/1]
		Scene の TileMap 側で SpriteRenderer 使うか頂点バッファに直接書き込むか決めたいので、
		仮想クラスとしておく。
*/
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMapModel.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/SpriteRenderer.h"

LN_NAMESPACE_BEGIN

struct TileMapVertex
{
	Vector3		Position;
	Vector2		TexUV;

	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		return elements;
	}
	static const int ElementCount = 2;
};

//==============================================================================
// TileMapRenderer
//==============================================================================

static const byte_t g_TileMapRenderer_fx_Data[] =
{
#include "TileMapRenderer.fx.h"
};
static const size_t g_TileMapRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_TileMapRenderer_fx_Data);

//------------------------------------------------------------------------------
TileMapRenderer::TileMapRenderer(detail::GraphicsManager* manager)
	: m_graphicsManager(manager)
	//, m_vertexBuffer(nullptr)
	//, m_indexBuffer(nullptr)
{
	int tileCount = 20 * 20;
	m_mesh = Object::MakeRef<MeshResource>(manager, ResourceUsage::Dynamic);
	m_mesh->ResizeVertexBuffer(tileCount * 4);
	m_mesh->ResizeIndexBuffer(tileCount * 6);
	m_maxTileCount = 100 * 100;

	m_shader.shader = LN_NEW Shader();
	m_shader.shader->Initialize(m_graphicsManager, g_TileMapRenderer_fx_Data, g_TileMapRenderer_fx_Len);
	m_shader.pass = m_shader.shader->GetTechniques()[0]->GetPasses()[0];
	m_shader.varWorldMatrix = m_shader.shader->FindVariable(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->FindVariable(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.shader->FindVariable(_T("g_texture"));
	//m_shader.varPixelStep = m_shader.shader->FindVariable(_T("g_pixelStep"));
}

//------------------------------------------------------------------------------
TileMapRenderer::~TileMapRenderer()
{
	//LN_SAFE_RELEASE(m_vertexBuffer);
	//LN_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_shader.shader);
}

//------------------------------------------------------------------------------
void TileMapRenderer::SetTransform(const Matrix& world, const Matrix& viewProj)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
	m_viewProj = viewProj;
}

//------------------------------------------------------------------------------
void TileMapRenderer::Draw(DrawList* context, TileMapModel* tileMap, const RectF& boundingRect, const ViewFrustum& cameraFrustum, int priority)
{
	LN_CHECK_ARG(tileMap != nullptr);
	m_context = context;
	//m_spriteRenderer = spriteRenderer;

	// TODO: 原点と正面方向
	Plane plane(Vector3(0, 0, 0), Vector3(0, 0, -1));
	Vector3 corners[8];
	Vector3 pt;
	cameraFrustum.GetCornerPoints(corners);

	// TileMap の平面とカメラの視錐台から描画するべき範囲を求める
	float l, t, r, b;
	for (int i = 0; i < 4; ++i)
	{
		plane.Intersects(corners[i], corners[i+4], &pt);
		if (i == 0)
		{
			l = pt.x;
			t = pt.y;
			r = pt.x;
			b = pt.y;
		}
		else
		{
			l = std::min(l, pt.x);
			r = std::max(r, pt.x);
			t = std::max(t, pt.y);
			b = std::min(b, pt.y);
		}
	}

	//printf("%f %f %f %f \n", l, t, r, b);

	// TODO: ローカル座標→タイルマップ配列インデックスへの座標変換

	// TODO: ↓いまのところ 3d 用
	const SizeI& tileSize = tileMap->GetTileSet()->GetTileSize();

	// テーブル配列のどこの範囲を描画したい？
	BoundingRect renderRange =
	{
		static_cast<int>(l / tileSize.width),
		static_cast<int>(t / tileSize.height),
		static_cast<int>(r / tileSize.width),
		static_cast<int>(b / tileSize.height),
	};
	//renderRange.left += 2;
	renderRange.top += 1;
	renderRange.bottom -= 1;
	renderRange.right += 1;
	//RectI renderRange = Rect::MakeFromBounds(
	//	(int)l / tileSize.Width,
	//	(int)b / tileSize.Height,
	//	(int)r / tileSize.Width,
	//	(int)t / tileSize.Height);
	//renderRange.X = (int)l / tileSize.Width;
	//renderRange.Y = (int)t / tileSize.Height;
	//renderRange.Width = renderRange.X + ((int)r / tileSize.Width);
	//renderRange.Height = renderRange.Y - ((int)b / tileSize.Height);


	Begin();
	for (TileLayer* layer : *tileMap->GetLayers())
	{
		BoundingRect clipd = renderRange;
		// TODO: 3D で Y0 を下端とする
		clipd.top = layer->GetSize().height - clipd.top;
		clipd.bottom = layer->GetSize().height - clipd.bottom;
		//clipd.Y = layer->GetSize().Height + clipd.Y;

		// ループしない
		// TODO:
		if (1)
		{
			const SizeI& size = layer->GetSize();
			if (clipd.left < 0) clipd.left = 0;
			if (clipd.top < 0) clipd.top = 0;
			if (clipd.right > size.width) clipd.right = size.width;		// 描画したいところ +1
			if (clipd.bottom > size.height) clipd.bottom = size.height;	// 描画したいところ +1
		}


		//printf("%d %d %d %d \n", renderRange.GetLeft(), renderRange.GetTop(), renderRange.GetRight(), renderRange.GetBottom());
		//printf("%d %d %d %d \n", clipd.left, clipd.top, clipd.right, clipd.bottom);

		DrawLayer(layer, boundingRect, tileMap->GetTileSet(), clipd, priority);
	}
	End();
}

//------------------------------------------------------------------------------
void TileMapRenderer::Begin()
{
}

//------------------------------------------------------------------------------
void TileMapRenderer::End()
{
	//m_spriteRenderer->Flush();
}

//------------------------------------------------------------------------------
//void TileMapRenderer::DrawTile(
//	const Vector3& position,
//	const Vector2& size,
//	Texture* texture,
//	const RectI& srcRect)
//{
//	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
//}

//------------------------------------------------------------------------------
void TileMapRenderer::DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet, const BoundingRect& renderRange, int priority)
{
	LN_CHECK_ARG(layer != nullptr);
	LN_CHECK_ARG(tileSet != nullptr);

	int tileCount = (renderRange.right - renderRange.left) * (renderRange.bottom - renderRange.top);
	tileCount = std::min(tileCount, m_maxTileCount);
	int allocedTileCount = m_mesh->GetVertexCount() / 4;
	if (tileCount > allocedTileCount)
	{
		m_mesh->ResizeVertexBuffer(tileCount * 4);
		m_mesh->ResizeIndexBuffer(tileCount * 6);

		// インデックスバッファは途中で変わらないので先に埋めておく
		for (int iTile = 0; iTile < tileCount; ++iTile)
		{
			int i = iTile * 6;
			int v = iTile * 4;
			m_mesh->SetIndex(i + 0, v + 0);
			m_mesh->SetIndex(i + 1, v + 1);
			m_mesh->SetIndex(i + 2, v + 2);
			m_mesh->SetIndex(i + 3, v + 2);
			m_mesh->SetIndex(i + 4, v + 1);
			m_mesh->SetIndex(i + 5, v + 3);
		}
	}






	const SizeI& tileSize = tileSet->GetTileSize();

	// boundingRect 全体を埋めるように描画するべきセル範囲を決める
	//int ox = boundingRect.X / tileSize.Width;
	//int oy = boundingRect.Y / tileSize.Height;
	//int w = (boundingRect.Width + tileSize.Width) / tileSize.Width;
	//int h = (boundingRect.Height + tileSize.Height) / tileSize.Height;

	Texture* tileSetTexture = tileSet->GetImageSource();
	Size invSize(
		1.0f / tileSetTexture->GetSize().width,
		1.0f / tileSetTexture->GetSize().height);

	Vector3 pos;
	Vector2 size;
	Texture* texture;
	RectI srcRect;
	const SizeI& layerSize = layer->GetSize();

	//ByteBuffer* buf = m_vertexBuffer->Lock();
	//TileMapVertex* vb = (TileMapVertex*)buf->GetData();

	m_mesh->Clear();

	Vertex virtices[4];
	for (Vertex& v : virtices) { v.normal = Vector3::UnitZ, v.color = Color::White; }

	int plotCount = 0;
	Vector3 offset(0/*layerSize.X * tileSize.Width*/, layerSize.height * tileSize.height, 0);
	Vector3 stepX(tileSize.width, 0, 0);
	Vector3 stepY(0, -tileSize.height, 0);	// 3D なので下方向へ
	float delta = 0.000001;	// TODO: DX9 特有？ほんのわずかな誤差で、縦横に線が見えてしまうことがあった
	for (int y = renderRange.top; y < renderRange.bottom; ++y)
	{
		Vector3 pos = offset + (stepY * y) + (stepX * renderRange.left);
		for (int x = renderRange.left; x < renderRange.right; ++x)
		{
			//if (x < layerSize.Width && y < layerSize.Height)
			{
				tileSet->LookupTileImage(layer->GetTileId(x, y), &texture, &srcRect);
				//float pl = (float)x;
				//float pt = (float)y;
				//float pr = (float)x + tileSize.Width;
				//float pb = (float)y + tileSize.Height;
				float tl = ((float)srcRect.GetLeft()) * invSize.width + delta;
				float tt = ((float)srcRect.GetTop()) * invSize.height + delta;
				float tr = ((float)srcRect.GetRight()) * invSize.width + delta;
				float tb = ((float)srcRect.GetBottom()) * invSize.height + delta;
				//pos.Set(x * tileSize.Width, y * tileSize.Height, 0);
				//size.Set(srcRect.Width, srcRect.Height);
				//DrawTile(pos, size, texture, srcRect);
				virtices[0].position = pos;
				virtices[0].uv.Set(tl, tt);
				virtices[1].position = pos + stepY;
				virtices[1].uv.Set(tl, tb);
				virtices[2].position = pos + stepX + stepY;
				virtices[2].uv.Set(tr, tb);
				virtices[3].position = pos + stepX;
				virtices[3].uv.Set(tr, tt);
				//plotCount += 4;

				m_mesh->AddSquare(virtices);
				plotCount++;

				if (plotCount > m_maxTileCount) goto LOOP_EXIT;

				//vb[plotCount + 0].Position.Set(0, 10, 0);
				//vb[plotCount + 0].TexUV.Set(tl, tt);
				//vb[plotCount + 1].Position.Set(0, 0, 0);
				//vb[plotCount + 1].TexUV.Set(tl, tb);
				//vb[plotCount + 2].Position.Set(-10, 10, 0);
				//vb[plotCount + 2].TexUV.Set(tr, tt);
				//vb[plotCount + 3].Position.Set(-10, 0, 0);
				//vb[plotCount + 3].TexUV.Set(tr, tb);
			}
			pos += stepX;
		}
	}

LOOP_EXIT:

	//printf("---\n");
	//Vector3::Transform(vb[0].Position, m_viewProj).Print();
	//Vector3::Transform(vb[1].Position, m_viewProj).Print();
	//Vector3::Transform(vb[2].Position, m_viewProj).Print();
	//Vector3::Transform(vb[3].Position, m_viewProj).Print();

	//m_vertexBuffer->Unlock();





	//m_renderingContext->Flush();


	// TODO: 複数テクスチャ
	// (と言っても、実際は1layerに1テクスチャでいいと思う。RGSSが少し特殊なだけ。最初に BitBlt で1つのテクスチャにまとめてしまってもいいかも)
	//m_shader.varTexture->SetTexture(tileSetTexture);
	//m_shader.varViewProjMatrix->SetMatrix(m_viewProj);



	//RenderState s = m_renderingContext->GetRenderState();
	//s.Culling = CullingMode_None;
	//m_renderingContext->SetRenderState(s);

	//m_context->SetShaderPass(m_shader.pass);
	//m_context->DrawPrimitiveIndexed(m_vertexDeclaration, m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, plotCount / 2);
	
	DrawElementMetadata metadata;
	metadata.priority = priority;
	m_context->PushMetadata(&metadata);
	m_context->DrawMesh(m_mesh, 0, tileSet->GetMaterial());
	//printf("%p\n", m_shader.pass);
	//m_renderingContext->SetVertexBuffer(nullptr);
	//m_renderingContext->SetIndexBuffer(nullptr);
	//m_renderingContext->SetShaderPass(nullptr);

}
//
////==============================================================================
//// SpriteTileMapRenderer
////==============================================================================
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//SpriteTileMapRenderer::SpriteTileMapRenderer(GraphicsManager* manager)
//	: m_spriteRenderer(nullptr)
//{
//	m_spriteRenderer = SpriteRenderer::Create(MaxSpriteCount, manager);
//	m_spriteRenderer->SetViewPixelSize(SizeI(640, 480));	// TODO
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//SpriteTileMapRenderer::~SpriteTileMapRenderer()
//{
//	LN_SAFE_RELEASE(m_spriteRenderer);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetTransform(const Matrix& matrix)
//{
//	m_spriteRenderer->SetTransform(matrix);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
//{
//	m_spriteRenderer->SetViewProjMatrix(view, proj);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetRenderState(const RenderState& state)
//{
//	m_spriteRenderer->SetRenderState(state);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::Begin()
//{
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::End()
//{
//	m_spriteRenderer->Flush();
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void SpriteTileMapRenderer::DrawTile(
//	const Vector3& position,
//	const Vector2& size,
//	Texture* texture,
//	const RectI& srcRect)
//{
//	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
//}


LN_NAMESPACE_END
