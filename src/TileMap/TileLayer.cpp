
#include "../Internal.h"
#include <Lumino/TileMap/TileLayer.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// TileLayer
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TileLayer, tr::ReflectionObject);
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileLayer::TileLayer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileLayer::~TileLayer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileLayer::Resize(int width, int height)
{
	m_size.Set(width, height);
	m_data.Resize(width * height);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileLayer::SetTileId(int x, int y, int id)
{
	m_data[y * m_size.Width + x];
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int TileLayer::GetTileId(int x, int y)
{
	return m_data[y * m_size.Width + x];
}

LN_NAMESPACE_END
