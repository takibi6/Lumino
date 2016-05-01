﻿
#include "../Internal.h"
#include <Lumino/GUI/GUIInputGesture.h>
#include <Lumino/GUI/RoutedCommand.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// InputGesture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputGesture::InputGesture()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputGesture::~InputGesture()
{
}


//=============================================================================
// KeyGesture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyGesture::KeyGesture(Key key)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyGesture::KeyGesture(Key key, ModifierKeys modifierKeys)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyGesture::~KeyGesture()
{
}


//=============================================================================
// UIInputBinding
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIInputBinding::UIInputBinding(Command* command, InputGesture* gesture)
	: m_command(NULL)
	, m_gesture(NULL)
{
	LN_REFOBJ_SET(m_command, command);
	LN_REFOBJ_SET(m_gesture, gesture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIInputBinding::~UIInputBinding()
{
	LN_SAFE_RELEASE(m_command);
	LN_SAFE_RELEASE(m_gesture);
}


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
