
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/TileMap/TileSet.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// TileSet
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileSet, tr::ReflectionObject);
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileSet::TileSet()
	: m_imageSource(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileSet::~TileSet()
{
	LN_SAFE_RELEASE(m_imageSource);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileSet::SetImageSource(Texture* texture)
{
	LN_REFOBJ_SET(m_imageSource, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* TileSet::GetImageSource() const
{
	return m_imageSource;
}

LN_NAMESPACE_END
