﻿
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Brush.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Brush
//==============================================================================

//------------------------------------------------------------------------------
Brush::Brush()
{
}

//------------------------------------------------------------------------------
Brush::~Brush()
{
}

//==============================================================================
// ColorBrush
//==============================================================================

// 以前は ColorF の static 変数を参照していたが、それだと初期化の順によってはこちらの値がすべて 0,0,0,0 になってしまうことがあった
static ColorBrush g_ColorBrush_White(1.0, 1.0, 1.0, 1.0);
static ColorBrush g_ColorBrush_Black(0.0, 0.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Gray(0.5, 0.5, 0.5, 1.0);
static ColorBrush g_ColorBrush_Red(1.0, 0.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Green(0.0, 1.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Blue(0.0, 0.0, 1.0, 1.0);
static ColorBrush g_ColorBrush_DimGray(0.25, 0.25, 0.25, 1.0);
ColorBrush*	ColorBrush::White = &g_ColorBrush_White;
ColorBrush*	ColorBrush::Black = &g_ColorBrush_Black;
ColorBrush*	ColorBrush::Gray = &g_ColorBrush_Gray;
ColorBrush*	ColorBrush::Red = &g_ColorBrush_Red;
ColorBrush*	ColorBrush::Green = &g_ColorBrush_Green;
ColorBrush*	ColorBrush::Blue = &g_ColorBrush_Blue;
ColorBrush*	ColorBrush::DimGray = &g_ColorBrush_DimGray;

//------------------------------------------------------------------------------
ColorBrush::ColorBrush(const Color32& color)
	: m_color(color)
{
}

//------------------------------------------------------------------------------
ColorBrush::ColorBrush(const Color& color)
	: m_color(color)
{
}

//------------------------------------------------------------------------------
ColorBrush::ColorBrush(float r, float g, float b, float a)
	: m_color(r, g, b, a)
{
}

//------------------------------------------------------------------------------
ColorBrush::~ColorBrush()
{
}

//==============================================================================
// TextureBrush
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(const StringRef& filePath)
{
	RefPtr<TextureBrush> obj(LN_NEW TextureBrush(), false);
	obj->Create(filePath.GetBegin(), nullptr);	// TODO: getBegin
	return obj;
}

//------------------------------------------------------------------------------
TextureBrush::TextureBrush()
	: m_srcRect(0, 0, INT_MAX, INT_MAX)
	, m_wrapMode(BrushWrapMode::Stretch)
	, m_imageDrawMode(BrushImageDrawMode::Image)
{
}

//------------------------------------------------------------------------------
TextureBrush::~TextureBrush()
{
}

//------------------------------------------------------------------------------
void TextureBrush::Create(const TCHAR* filePath, detail::GraphicsManager* manager)
{
	m_texture = Texture2D::Create(filePath, TextureFormat::R8G8B8A8, false);		//TODO: GraphicsManager?
}

//------------------------------------------------------------------------------
void TextureBrush::Create(Texture* texture)
{
	m_texture = texture;
}


//------------------------------------------------------------------------------
void TextureBrush::SetTexture(Texture* texture)
{
	m_texture = texture;
}

//------------------------------------------------------------------------------
Texture* TextureBrush::GetTexture() const
{
	return m_texture;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

