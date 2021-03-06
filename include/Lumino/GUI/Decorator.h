﻿
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		1 つの子要素の周囲を装飾する UI 要素の基本クラスです。
*/
class Decorator
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Decorator);
public:
	Decorator(GUIManagerImpl* manager);
	virtual ~Decorator();

public:
	void SetChild(UIElement* element);
	UIElement* GetChild() const;

public:
	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }


protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);;
	//virtual void AddChildToVisualTree(UIElement* element);

private:
	RefPtr<UIElement>	m_child;
	//RefPtr<UIElement>	m_visualChild;
};


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
