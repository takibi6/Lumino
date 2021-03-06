﻿
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"
#include "LayoutImpl.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UILayoutPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UILayoutPanel, UIElement)

//------------------------------------------------------------------------------
UILayoutPanel::UILayoutPanel()
{
}

//------------------------------------------------------------------------------
UILayoutPanel::~UILayoutPanel()
{
}

//------------------------------------------------------------------------------
void UILayoutPanel::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
	m_children = RefPtr<UIElementCollection>::MakeRef(this);

	// Panel 系のデフォルトは Stretch
	SetHAlignment(HAlignment::Stretch);
	SetVAlignment(VAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UILayoutPanel::AddChild(UIElement* element)
{
	m_children->Add(element);
}

//------------------------------------------------------------------------------
void UILayoutPanel::RemoveChild(UIElement* element)
{
	m_children->Remove(element);
}

//------------------------------------------------------------------------------
int UILayoutPanel::GetVisualChildrenCount() const
{
	return m_children->GetCount();
}

//------------------------------------------------------------------------------
UIElement* UILayoutPanel::GetVisualChild(int index) const
{
	return m_children->GetAt(index);
}

//------------------------------------------------------------------------------
Size UILayoutPanel::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_MeasureOverride(
		this, constraint,
		[](UILayoutPanel* panel, const Size& constraint){ return panel->UIElement::MeasureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UILayoutPanel::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_ArrangeOverride(this, finalSize);
}

//------------------------------------------------------------------------------
void UILayoutPanel::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	// 新しく追加されたものたち
	for (UIElement* element : e.newItems)
	{
		element->SetParent(this);
	}

	// 削除されたものたち
	for (UIElement* element : e.oldItems)
	{
		element->SetParent(nullptr);
	}
}

//------------------------------------------------------------------------------
int UILayoutPanel::GetLayoutChildrenCount() { return m_children->GetCount(); }
ILayoutElement* UILayoutPanel::GetLayoutChild(int index) { return m_children->GetAt(index); }
int UILayoutPanel::GetLayoutGridColumnDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::GetLayoutGridColumnDefinition(int index) { return nullptr; }
int UILayoutPanel::GetLayoutGridRowDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::GetLayoutGridRowDefinition(int index) { return nullptr; }

//==============================================================================
// UIStackPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIStackPanel, UILayoutPanel)

//------------------------------------------------------------------------------
UIStackPanelPtr UIStackPanel::Create()
{
	auto ptr = RefPtr<UIStackPanel>::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIStackPanel::UIStackPanel()
	: m_orientation(Orientation::Vertical)
{
}

//------------------------------------------------------------------------------
UIStackPanel::~UIStackPanel()
{
}

//------------------------------------------------------------------------------
void UIStackPanel::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
Size UIStackPanel::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UIStackPanel>::UIStackPanel_MeasureOverride(this, constraint, m_orientation);
}

//------------------------------------------------------------------------------
Size UIStackPanel::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIStackPanel>::UIStackPanel_ArrangeOverride(this, finalSize, m_orientation);
}


//==============================================================================
// UICanvas
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UICanvas, UILayoutPanel)

//------------------------------------------------------------------------------
UICanvasPtr UICanvas::Create()
{
	auto ptr = UICanvasPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UICanvas::UICanvas()
{
}

//------------------------------------------------------------------------------
UICanvas::~UICanvas()
{
}

//------------------------------------------------------------------------------
void UICanvas::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
Size UICanvas::MeasureOverride(const Size& constraint)
{
	return UILayoutPanel::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UICanvas::ArrangeOverride(const Size& finalSize)
{
	//ThicknessF canvas;
	
	
	for (UIElement* child : *GetChildren())
	{
		//if (child->GetInvalidateFlags().TestFlag(detail::InvalidateFlags::ParentChangedUpdating))
		//{
		//}
		
		const Size& desiredSize = child->GetDesiredSize();
		Size size = child->GetSizeInternal();
		size.width = Math::IsNaN(size.width) ? desiredSize.width : size.width;
		size.height = Math::IsNaN(size.height) ? desiredSize.height : size.height;

		RectF childRect(child->GetPositionInternal(), child->GetSizeInternal());
		AlignmentAnchor anchor = child->GetAnchorInternal();
		
		if (anchor != AlignmentAnchor::None)
		{
			const ThicknessF& margin = GetMargineInternal();
			
			float l = NAN, t = NAN, r = NAN, b = NAN;
			if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
				l = margin.Left;
			else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
				l = finalSize.width * margin.Left;
			
			if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
				t = margin.Top;
			else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
				t = finalSize.height * margin.Top;
			
			if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
				r = finalSize.width - margin.Right;
			else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				r = finalSize.width - (finalSize.width * margin.Right);
			
			if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
				b = finalSize.height - margin.Bottom;
			else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				b = finalSize.height - (finalSize.height * margin.Bottom);

			if (anchor.TestFlag(AlignmentAnchor::HCenter))
				childRect.x = (finalSize.width - childRect.width) / 2;

			if (anchor.TestFlag(AlignmentAnchor::VCenter))
				childRect.y = (finalSize.height - childRect.height) / 2;

			if (!Math::IsNaN(l) || !Math::IsNaN(r))
			{
				if (!Math::IsNaN(l) && Math::IsNaN(r))
				{
					childRect.x = l;
				}
				else if (Math::IsNaN(l) && !Math::IsNaN(r))
				{
					childRect.x = r - childRect.width;
				}
				else
				{
					childRect.x = l;
					childRect.width = r - l;
				}
			}

			if (!Math::IsNaN(t) || !Math::IsNaN(b))
			{
				if (!Math::IsNaN(t) && Math::IsNaN(b))
				{
					childRect.y = t;
				}
				else if (Math::IsNaN(t) && !Math::IsNaN(b))
				{
					childRect.y = b - childRect.height;
				}
				else
				{
					childRect.y = t;
					childRect.height = b - t;
				}
			}

			child->ArrangeLayout(childRect);
		}
		else
		{
			child->ArrangeLayout(childRect);
		}
	}

	return finalSize;
}


//==============================================================================
// DefinitionBase
//==============================================================================
class UIGridLayout::DefinitionBase
	: public Object
{
public:

	DefinitionBase()
	{
	}

	virtual ~DefinitionBase()
	{
	}

	GridLengthType GetType() const { return m_data.type; }

	//float GetAvailableDesiredSize() const
	//{
	//	if (m_data.type == GridLengthType::Auto) {
	//		return m_data.desiredSize;
	//	}
	//	else if (m_data.type == GridLengthType::Pixel) {
	//		return Math::Clamp(m_data.size, m_data.minSize, m_data.maxSize);
	//	}
	//	else {
	//		return 0;
	//	}
	//}

	//float GetRatioWeight() const
	//{
	//	return (m_data.size == 0.0f) ? 1.0f : m_data.size;
	//}

	//void AdjustActualSize()
	//{
	//	m_data.actualSize = Math::Clamp(m_data.actualSize, m_data.minSize, m_data.maxSize);
	//}

LN_INTERNAL_ACCESS:
	detail::GridDefinitionData	m_data;
//	GridLengthType	m_type;
//	float			m_size;
//	float			m_minSize;
//	float			m_maxSize;
//
//public:
//	float			m_desiredSize;
//	float			m_actualOffset;	// 最終オフセット
//	float			m_actualSize;	// 最終サイズ
};

//==============================================================================
// ColumnDefinition
//==============================================================================
class UIGridLayout::ColumnDefinition
	: public DefinitionBase
{
public:

	ColumnDefinition()
	{
	}

	virtual ~ColumnDefinition()
	{
	}

	void SetWidth(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float GetWidth() const { return m_data.size; }

	void SetMinWidth(float value) { m_data.minSize = value; }
	float GetMinWidth() const { return m_data.minSize; }

	void SetMaxWidth(float value) { m_data.maxSize = value; }
	float GetMaxWidth() const { return m_data.maxSize; }
};

//==============================================================================
// RowDefinition
//==============================================================================
class UIGridLayout::RowDefinition
	: public DefinitionBase
{
public:

	RowDefinition()
	{
	}

	virtual ~RowDefinition()
	{
	}

	void SetHeight(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float GetHeight() const { return m_data.size; }

	void SetMinHeight(float value) { m_data.minSize = value; }
	float GetMinHeight() const { return m_data.minSize; }

	void SetMaxHeight(float value) { m_data.maxSize = value; }
	float GetMaxHeight() const { return m_data.maxSize; }
};

//==============================================================================
// UIGridLayout
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIGridLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create()
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create(int columnCount, int rowCount)
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	ptr->SetGridSize(columnCount, rowCount);
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayout::UIGridLayout()
	: m_columnDefinitions()
	, m_rowDefinitions()
{
}

//------------------------------------------------------------------------------
UIGridLayout::~UIGridLayout()
{
}

//------------------------------------------------------------------------------
void UIGridLayout::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIGridLayout::SetGridSize(int columnCount, int rowCount)
{
	m_columnDefinitions.Clear();
	m_rowDefinitions.Clear();

	for (int i = 0; i < columnCount; ++i)
		m_columnDefinitions.Add(RefPtr<ColumnDefinition>::MakeRef());
	for (int i = 0; i < rowCount; ++i)
		m_rowDefinitions.Add(RefPtr<RowDefinition>::MakeRef());
}

//------------------------------------------------------------------------------
void UIGridLayout::AddColumnDefinition(GridLengthType type, float width, float minWidth, float maxWidth)
{
	auto ptr = RefPtr<ColumnDefinition>::MakeRef();
	ptr->SetWidth(width, type);
	ptr->SetMinWidth(minWidth);
	ptr->SetMaxWidth(maxWidth);
	m_columnDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
void UIGridLayout::AddRowDefinition(GridLengthType type, float height, float minHeight, float maxHeight)
{
	auto ptr = RefPtr<RowDefinition>::MakeRef();
	ptr->SetHeight(height, type);
	ptr->SetMinHeight(minHeight);
	ptr->SetMaxHeight(maxHeight);
	m_rowDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
Size UIGridLayout::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_MeasureOverride(
		this, constraint,
		[](UIGridLayout* panel, const Size& constraint){ return panel->UILayoutPanel::MeasureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UIGridLayout::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_ArrangeOverride(this, finalSize);
}

//------------------------------------------------------------------------------
int UIGridLayout::GetLayoutGridColumnDefinitionCount() { return m_columnDefinitions.GetCount(); }
detail::GridDefinitionData* UIGridLayout::GetLayoutGridColumnDefinition(int index) { return &m_columnDefinitions[index]->m_data; }
int UIGridLayout::GetLayoutGridRowDefinitionCount() { return m_rowDefinitions.GetCount(); }
detail::GridDefinitionData* UIGridLayout::GetLayoutGridRowDefinition(int index) { return &m_rowDefinitions[index]->m_data; }

LN_NAMESPACE_END
