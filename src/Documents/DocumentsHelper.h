﻿
#pragma once
#include <Lumino/Documents/TextElement.h>

namespace Lumino
{
namespace Documents
{

class DocumentsHelper
{
public:
	static void TextElement_SetParent(TextElement* obj, TextElement* value) { obj->m_parent = value; }
};


} // namespace Documents
} // namespace Lumino
