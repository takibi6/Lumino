﻿
#pragma once
#include "Common.h"
#include "../Graphics/Mesh.h"

LN_NAMESPACE_BEGIN
class DrawList;

/**
	@brief	
*/
class TileMapRenderer
	: public RefObject
{
public:
	void SetTransform(const Matrix& world, const Matrix& viewProj);
	void Draw(DrawList* context, TileMapModel* tileMap, const RectF& boundingRect, const ViewFrustum& cameraFrustum, int priority);

protected:
	void Begin();

	void End();

	//void DrawTile(
	//	const Vector3& position,
	//	const Vector2& size,
	//	Texture* texture,
	//	const RectI& srcRect);

public:
	TileMapRenderer(detail::GraphicsManager* manager);
	virtual ~TileMapRenderer();

private:
	struct BoundingRect
	{
		int	left;
		int	top;
		int	right;
		int	bottom;
	};

	void DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet, const BoundingRect& renderRange, int priority);

	detail::GraphicsManager*	m_graphicsManager;
	DrawList*	m_context;
	RefPtr<MeshResource>	m_mesh;
	int						m_maxTileCount;
	//RefPtr<VertexDeclaration>	m_vertexDeclaration;
	//VertexBuffer*		m_vertexBuffer;
	//IndexBuffer*		m_indexBuffer;
	Matrix				m_viewProj;

	//SpriteRenderer*	m_spriteRenderer;


	struct
	{
		Shader*			shader;
		ShaderPass*		pass;
		ShaderVariable*	varWorldMatrix;
		ShaderVariable*	varViewProjMatrix;
		//ShaderVariable*	varPixelStep;
		ShaderVariable*	varTexture;

	} m_shader;
};

///**
//	@brief	
//*/
//class SpriteTileMapRenderer
//	: public TileMapRenderer
//{
//public:
//	SpriteTileMapRenderer(GraphicsManager* manager);
//	virtual ~SpriteTileMapRenderer();
//
//	/**
//		@brief		座標変換行列を設定します。
//		@param[in]	matrix		: 座標変換行列
//	*/
//	void SetTransform(const Matrix& matrix);
//
//	/**
//		@brief		ビュー及びプロジェクション行列を設定します。
//	*/
//	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);
//
//	/**
//		@brief		レンダリングステートを設定します。
//	*/
//	void SetRenderState(const RenderState& state);
//
//protected:
//	virtual void Begin();
//	virtual void End();
//	virtual void DrawTile(
//		const Vector3& position,
//		const Vector2& size,
//		Texture* texture,
//		const RectI& srcRect);
//
//private:
//	const int MaxSpriteCount = 2048;
//	SpriteRenderer*	m_spriteRenderer;
//};

LN_NAMESPACE_END
