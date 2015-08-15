
#include "../Internal.h"
#include <Lumino/GUI/ButtonChrome.h>
#include <Lumino/GUI/VisualState.h>	// for button

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ButtonChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonChrome, Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonChrome);

// Register property
LN_PROPERTY_IMPLEMENT(ButtonChrome, bool, IsMouseOverProperty, "IsMouseOver", m_isMouseOver, false, NULL);
LN_PROPERTY_IMPLEMENT(ButtonChrome, float, FrameWidthProperty, "FrameWidth", m_frameWidth, 8.0f, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChromePtr ButtonChrome::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8.0f)
	, m_isMouseOver(false)
{
	// �v���p�e�B�̓o�^
	//LN_DEFINE_PROPERTY(ButtonChrome, bool, IsMouseOverProperty, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver, false);

	// TODO
	// �{�^���̃X�^�C���ƃe���v���[�g
	// https://msdn.microsoft.com/ja-jp/library/ms753328%28v=vs.110%29.aspx
	RefPtr<VisualStateManager> vsm(LN_NEW VisualStateManager());

	RefPtr<VisualStateGroup> group1(LN_NEW VisualStateGroup(_T("CommonStates")));


	RefPtr<VisualState> state1(LN_NEW VisualState(_T("Normal")));

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::~ButtonChrome()
{
}
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void ButtonChrome::SetPropertyValue(const Property* prop, const Variant& value)
//{
//	if (prop == FrameWidthProperty) { m_frameWidth = value.GetFloat(); return; }
//	Decorator::SetPropertyValue(prop, value);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//Variant ButtonChrome::GetPropertyValue(const Property* prop) const
//{
//	if (prop == FrameWidthProperty) { return Variant(m_frameWidth); }
//	return Decorator::GetPropertyValue(prop);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_brush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalFrameBrush")));
	m_bgBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalBackgroundBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnRender(Graphics::Painter* painter)
{
	RectF bgRect = m_finalLocalRect;
	RectF rect = m_finalLocalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter->SetBrush(m_bgBrush);
		painter->DrawRectangle(bgRect);
	}

	painter->SetBrush(m_brush);
	painter->DrawFrameRectangle(rect, m_frameWidth);
}

} // namespace GUI
} // namespace Lumino