﻿
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIElementCollection.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIElementCollection
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIElementCollection, tr::ReflectionObjectList<UIElement*>);

//------------------------------------------------------------------------------
UIElementCollection::UIElementCollection(tr::IUIElementCollectionOwner* owner)
	: m_owner(owner)
{
}

//------------------------------------------------------------------------------
UIElementCollection::~UIElementCollection()
{
}

//------------------------------------------------------------------------------
void UIElementCollection::InsertItem(int index, const value_type& item)
{
	m_newItemsCache.Resize(1);
	m_newItemsCache[0] = item;
	m_oldItemsCache.Clear();

	tr::ReflectionObjectList<UIElement*>::InsertItem(index, item);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Add,
		m_newItemsCache, index,
		m_oldItemsCache, 0,
	};
	m_owner->OnChildCollectionChanged(e);
}

//------------------------------------------------------------------------------
void UIElementCollection::ClearItems()
{
	m_newItemsCache.Clear();
	m_oldItemsCache.Clear();

	tr::ReflectionObjectList<UIElement*>::ClearItems();

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Reset,
		m_newItemsCache, 0,
		m_oldItemsCache, 0,
	};
	m_owner->OnChildCollectionChanged(e);
}

//------------------------------------------------------------------------------
void UIElementCollection::RemoveItem(int index)
{
	m_newItemsCache.Clear();
	m_oldItemsCache.Resize(1);
	m_oldItemsCache[0] = GetAt(index);

	tr::ReflectionObjectList<UIElement*>::RemoveItem(index);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Remove,
		m_newItemsCache, 0,
		m_oldItemsCache, index,
	};
	m_owner->OnChildCollectionChanged(e);
}

//------------------------------------------------------------------------------
void UIElementCollection::SetItem(int index, const value_type& item)
{
	m_newItemsCache.Resize(1);
	m_newItemsCache[0] = item;
	m_oldItemsCache.Resize(1);
	m_oldItemsCache[0] = GetAt(index);

	tr::ReflectionObjectList<UIElement*>::SetItem(index, item);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Replace,
		m_newItemsCache, index,
		m_oldItemsCache, index,
	};
	m_owner->OnChildCollectionChanged(e);
}

LN_NAMESPACE_END

