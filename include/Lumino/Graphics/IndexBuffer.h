﻿
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		インデックスバッファのクラスです。
*/
class IndexBuffer
	: public GraphicsResourceObject
{
public:

	/**
		@brief		インデックスバッファを作成します。
		@param[in]	indexCount		: インデックスの要素数
		@param[in]	initialData		: 初期値として設定するインデックスデータ
		@param[in]	format			: インデックスバッファのフォーマット
		@param[in]	usage			: インデックスバッファリソースの使用方法
	*/
	//static IndexBuffer* Create(int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);
	
	/**
		@brief		インデックスバッファを作成します。
		@param[in]	manager			: 作成に使用する GraphicsManager
		@param[in]	indexCount		: インデックスの要素数
		@param[in]	initialData		: 初期値として設定するインデックスデータ
		@param[in]	format			: インデックスバッファのフォーマット
		@param[in]	usage			: インデックスバッファリソースの使用方法
		@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
	*/
	//static IndexBuffer* Create(GraphicsManager* manager, int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);

public:

	/**
		@brief		インデックスデータ1つあたりのバイト数を返します。
	*/
	int GetIndexStride() const;

	/**
		@brief		リソースをロックします。
	*/
	ByteBuffer* Lock();

	/**
		@brief		リソースをアンロックします。
	*/
	void Unlock();

	/*
		@brief		インデックスデータを更新します。
		@param[in]	offsetBytes		: 更新先領域の先頭オフセット (バイト単位)
		@param[in]	data			: 転送するデータの先頭アドレス
		@param[in]	dataBytes		: 転送するデータのバイト数
	*/
	//void SetSubData(uint32_t offsetBytes, void* data, uint32_t dataBytes);

LN_INTERNAL_ACCESS:
	IndexBuffer();
	virtual ~IndexBuffer();
	void Initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage);

	Driver::IIndexBuffer* GetDeviceObject() const;// { return m_deviceObj; }
	int GetIndexCount() const { return m_indexCount; }
	IndexBufferFormat GetIndexFormat() const { return m_format; }
	void Resize(int indexCount, IndexBufferFormat format);

	// GraphicsResourceObject interface
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	//Driver::IIndexBuffer*	m_deviceObj;
	RefPtr<Driver::IIndexBuffer>	m_deviceObj;
	int						m_indexCount;
	IndexBufferFormat		m_format;
	ResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	//ByteBuffer				m_data;
	ByteBuffer				m_lockedBuffer;
	bool					m_initialUpdate;
	friend class Helper;
};

/**
	@brief		IndexBuffer のリソースロックを補助します。
*/
class ScopedIndexBufferLock
{
public:
	ScopedIndexBufferLock(IndexBuffer* indexBuffer)
	{
		m_indexBuffer = indexBuffer;
		m_lockedBuffer = m_indexBuffer->Lock();
	}
	
	~ScopedIndexBufferLock()
	{
		m_indexBuffer->Unlock();
	}

	byte_t* GetData()
	{
		return m_lockedBuffer->GetData();
	}

	size_t GetSize()
	{
		return m_lockedBuffer->GetSize();
	}

private:
	IndexBuffer*	m_indexBuffer;
	ByteBuffer*		m_lockedBuffer;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
