﻿
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SceneNode.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// SceneNodeRefList
//=============================================================================
//LN_REF_OBJECT_LIST_IMPL(SceneNodeRefList, SceneNode);
//
//
//SceneNodeRefList::iterator::reference SceneNodeRefList::iterator::operator*() const { return static_cast<reference>(*m_internalItr); }
//SceneNodeRefList::iterator::reference SceneNodeRefList::iterator::operator[](difference_type offset) const { return static_cast<reference>(m_internalItr[offset]); }

//=============================================================================
// SceneNode
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(SceneNode, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneNode::SceneNode()
	: m_manager(NULL)
	, m_ownerSceneGraph(nullptr)
	, m_name()
	, m_localMatrix()
	, m_transform()
	, m_transformCenter()
	, m_rotOrder(RotationOrder_XYZ)
	, m_priority(0)
	, m_billboardType(BillboardType_None)
	, m_transformModified(true)
	, m_isAutoUpdate(false)
	, m_children(LN_NEW SceneNodeRefList(), false)
	, m_parentNode(NULL)
	, m_zDistance(FLT_MAX)
{
	memset(m_renderingPassClientDataList, 0, sizeof(m_renderingPassClientDataList));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneNode::~SceneNode()
{
	if (m_ownerSceneGraph != nullptr) {
		m_ownerSceneGraph->RemoveNode(this);
	}
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneNode::CreateCore(SceneGraphManager* manager)
{
	LN_REFOBJ_SET(m_manager, manager);
	//m_manager->AddNode(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneNode::SetName(const String& name)
{
	if (m_name != name)
	{
		m_manager->OnNodeRename(this, m_name, name);
		m_name = name;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneNode::AddChild(SceneNode* child)
{
	LN_THROW(child != NULL, ArgumentException);
	LN_THROW(child->m_parentNode == NULL, InvalidOperationException);	// 既に別のノードの子になっている

	if (child->m_ownerSceneGraph != nullptr) {
		child->m_ownerSceneGraph->RemoveNode(this);
		child->m_ownerSceneGraph = nullptr;
	}

	m_children->Add(child);
	child->m_parentNode = this;

	child->m_ownerSceneGraph = m_ownerSceneGraph;
	child->m_ownerSceneGraph->AddNode(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneNode::UpdateFrameHierarchy(SceneNode* parent)
{
	// ワールド行列の更新が必要な場合は再計算
	if (m_transformModified)
	{
		m_localMatrix = Matrix::Identity;
		m_localMatrix.Translation(-m_transformCenter.X, -m_transformCenter.Y, -m_transformCenter.Z);
		m_localMatrix.Scaling(m_transform.Scale);
		m_localMatrix.RotateQuaternion(m_transform.Rotation);
		m_localMatrix.Translate(m_transform.Translation);
		m_transformModified = false;
	}

	// グローバル行列結合
	if (parent != NULL) {
		m_combinedGlobalMatrix = m_localMatrix * parent->m_combinedGlobalMatrix;
	}
	else {
		m_combinedGlobalMatrix = m_localMatrix;
	}

	// 子ノード更新
	LN_FOREACH(SceneNode* child, *m_children) {
		child->UpdateFrameHierarchy(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneNode::UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList)
{
	// SceneNode 自体は描画機能を持たないので何もせず、子の処理を行う
	LN_FOREACH(SceneNode* child, *m_children) {
		child->UpdateViewFlustumHierarchy(camera, renderingNodeList, renderingLightList);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool SceneNode::CmpZAndPrioritySort(const SceneNode* left, const SceneNode* right)
{
	if (left->m_priority == right->m_priority)
	{
		// 距離は昇順。近いほうを先に描画する。
		return left->m_zDistance < right->m_zDistance;
	}
	// 優先度は降順。高いほうを先に描画する。
	return left->m_priority < right->m_priority;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
