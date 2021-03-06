﻿
#if 0

#pragma once
#include "Internal.h"
#include <Lumino/RoutedEvent.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// RoutedEventArgs
//==============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(RoutedEventArgs, CoreObject);

//------------------------------------------------------------------------------
RoutedEventArgs::RoutedEventArgs()
	: /*HandlerOwner(NULL)
	,*/ Handled(false)
{
}

//------------------------------------------------------------------------------
RoutedEventArgs::~RoutedEventArgs()
{
}

//==============================================================================
// PropertyChangedEventArgs
//==============================================================================
//
LN_CORE_OBJECT_TYPE_INFO_IMPL(PropertyChangedEventArgs, CoreObject);	// キャッシュできるようにするため、実装しておく
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//PropertyChangedEventArgs::PropertyChangedEventArgs(const String& propertyName, const Variant& newValue)
//	: PropertyName(propertyName)
//	, NewValue(newValue)
//{
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//PropertyChangedEventArgs::~PropertyChangedEventArgs()
//{
//}

LN_NAMESPACE_END

#endif
