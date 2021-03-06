﻿
#include "../Internal.h"
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// InputBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(InputBinding, tr::ReflectionObject);

//------------------------------------------------------------------------------
InputBinding::InputBinding()
	: m_minValidMThreshold(0.2f)
{
}

//------------------------------------------------------------------------------
InputBinding::~InputBinding()
{
}

//==============================================================================
// KeyboardBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(KeyboardBinding, InputBinding);

//------------------------------------------------------------------------------
KeyboardBindingPtr KeyboardBinding::Create(Keys key, ModifierKeys modifierKeys)
{
	auto ptr = KeyboardBindingPtr::MakeRef(key, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
KeyboardBinding::KeyboardBinding(Keys key, ModifierKeys modifierKeys)
	: m_key(key)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
KeyboardBinding::~KeyboardBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType KeyboardBinding::GetType() const
{
	return detail::InputBindingType::Keyboard;
}


//==============================================================================
// MouseBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MouseBinding, InputBinding);

//------------------------------------------------------------------------------
MouseBindingPtr MouseBinding::Create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	auto ptr = MouseBindingPtr::MakeRef(mouseAction, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
MouseBinding::MouseBinding(MouseAction mouseAction, ModifierKeys modifierKeys)
	: m_mouseAction(mouseAction)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
MouseBinding::~MouseBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType MouseBinding::GetType() const
{
	return detail::InputBindingType::Mouse;
}


//==============================================================================
// GamepadBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GamepadBinding, InputBinding);

//------------------------------------------------------------------------------
GamepadBindingPtr GamepadBinding::Create(GamepadElement element)
{
	auto ptr = GamepadBindingPtr::MakeRef(element);
	return ptr;
}

//------------------------------------------------------------------------------
GamepadBinding::GamepadBinding(GamepadElement element)
	: m_element(element)
{
}

//------------------------------------------------------------------------------
GamepadBinding::~GamepadBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType GamepadBinding::GetType() const
{
	return detail::InputBindingType::Gamepad;
}

LN_NAMESPACE_END
