﻿
#include "../../../Internal.h"
#include "DX9GraphicsDevice.h"
#include "DX9IndexBuffer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// DX9IndexBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9IndexBuffer::DX9IndexBuffer()
	: m_graphicsDevice(NULL)
	, m_format(IndexBufferFormat_UInt16)
	, m_indexBuffer(NULL)
	, m_indices(NULL)
	, m_indexCount(0)
	, m_indexStride(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9IndexBuffer::~DX9IndexBuffer()
{
	LN_SAFE_DELETE_ARRAY(m_indices);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9IndexBuffer::Create(DX9GraphicsDevice* device, int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	m_format = format;
	m_indexCount = indexCount;
	m_indexStride = (m_format == IndexBufferFormat_UInt16) ? 2 : 4;

	// dynamic
	if (usage == DeviceResourceUsage_Dynamic)
	{
		if (m_format == IndexBufferFormat_UInt16) {
			m_indices = LN_NEW uint16_t[m_indexCount];
		}
		else {
			m_indices = LN_NEW uint32_t[m_indexCount];
		}
	}
	// static
	else
	{
		LN_COMCALL(
			m_graphicsDevice->GetIDirect3DDevice9()->CreateIndexBuffer(
			m_indexStride * m_indexCount,
			0,
			(m_format == IndexBufferFormat_UInt16) ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
			D3DPOOL_MANAGED,
			&m_indexBuffer,
			NULL));
	}

	// data コピー
	if (initialData)
	{
		if (m_indexBuffer != NULL) {
			void* buf = NULL;
			LN_COMCALL(m_indexBuffer->Lock(0, 0, &buf, D3DLOCK_DISCARD));
			memcpy(buf, initialData, m_indexStride * m_indexCount);
			LN_COMCALL(m_indexBuffer->Unlock());
		}
		else {
			memcpy(m_indices, initialData, m_indexStride * m_indexCount);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* DX9IndexBuffer::Lock()
{
	if (m_indexBuffer != NULL)
	{
		void* r = NULL;
		LN_COMCALL(m_indexBuffer->Lock(0, 0, &r, 0));
		return r;
	}
	else
	{
		return m_indices;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9IndexBuffer::Unlock()
{
	if (m_indexBuffer != NULL) {
		LN_COMCALL(m_indexBuffer->Unlock());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9IndexBuffer::SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes)
{
	LN_THROW(0, NotImplementedException);
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
