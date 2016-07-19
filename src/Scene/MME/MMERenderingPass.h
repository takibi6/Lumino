﻿
#pragma once
#include "../RenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// MMDPass をキーとしてシェーダテクニックを検索する RenderingPass
class MMERenderingPass
	: public RenderingPass
{
public:
	// ownerShader: オフスクリーンシーンを作った時のもととなったシェーダ
	MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, Shader* ownerShader = NULL);
	virtual ~MMERenderingPass();

private:
	virtual void RenderNode(SceneGraphRenderingContext* dc, SceneNode* node);
	virtual void RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset) override;

private:
	/// このパスのデフォルト&優先シェーダと、指定したノードのシェーダから描画に使用するテクニックを選択する。
	void SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams);

private:
	MMDPass		m_mmdPass;
	Shader*	m_ownerShader;			///< このパス (OFFSCREENRENDERTARGET) の生成元となったシェーダ
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
