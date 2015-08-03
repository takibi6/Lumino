﻿
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/Texture.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/Utils.h>
#include "RenderingCommand.h"
#include "RenderingThread.h"
#include "GraphicsHelper.h"

namespace Lumino
{
namespace Graphics
{
	
//=============================================================================
// Texture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const Size& size, TextureFormat format, int mipLevels, GraphicsManager* manager)
{
	manager = (manager != NULL) ? manager : Internal::Manager;
	auto* device = Helper::GetGraphicsDevice(manager);

	// テクスチャを作る
	RefPtr<Device::ITexture> obj(device->CreateTexture(size, mipLevels, format));

	// ロック用のビットマップを作る
	RefPtr<Imaging::Bitmap> bitmap(LN_NEW Imaging::Bitmap(size, Utils::TranslatePixelFormat(format)));

	// TODO: この addref はコンストラクタ内でやるほうが自然かも
	obj.SafeAddRef();
	return LN_NEW Texture(manager, obj, bitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const TCHAR* filePath, TextureFormat format, int mipLevels, GraphicsManager* manager)
{
	manager = (manager != NULL) ? manager : Internal::Manager;
	auto* device = Helper::GetGraphicsDevice(manager);
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath));
	return Create(stream, format, mipLevels, manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(Stream* stream, TextureFormat format, int mipLevels, GraphicsManager* manager)
{
	manager = (manager != NULL) ? manager : Internal::Manager;
	auto* device = Helper::GetGraphicsDevice(manager);

	if (Helper::IsPlatformTextureLoading(manager))
	{
		RefPtr<Device::ITexture> obj(device->CreateTexturePlatformLoading(stream, mipLevels, format));
		if (!obj.IsNull())
		{
			RefPtr<Imaging::Bitmap> bitmap(LN_NEW Imaging::Bitmap(obj->GetSize(), Utils::TranslatePixelFormat(format)));
			obj.SafeAddRef();
			return LN_NEW Texture(manager, obj, bitmap);
		}
	}

	// ビットマップを作る
	RefPtr<Imaging::Bitmap> bitmap(LN_NEW Imaging::Bitmap(stream));

	// テクスチャを作る
	RefPtr<Device::ITexture> obj(device->CreateTexture(bitmap->GetSize(), mipLevels, format));

	// ビットマップを転送する
	Device::IGraphicsDevice::ScopedLockContext lock(device);
	obj->SetSubData(Point(0, 0), bitmap->GetBitmapBuffer()->GetConstData(), obj->GetSize());

	// TODO: primarySurface のフォーマットは、format に合わせて変換するべきかも
	obj.SafeAddRef();
	return LN_NEW Texture(manager, obj, bitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const void* data, size_t size, TextureFormat format, int mipLevels, GraphicsManager* manager)
{
	manager = (manager != NULL) ? manager : Internal::Manager;
	auto* device = Helper::GetGraphicsDevice(manager);
	MemoryStream stream;
	stream.Create(data, size);
	return Create(&stream, format, mipLevels, manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::CreateRenderTarget(const Size& size, int mipLevels, TextureFormat format)
{
	LN_THROW(Internal::Manager != NULL, InvalidOperationException);
	Device::ITexture* obj = Helper::GetGraphicsDevice(Internal::Manager)->CreateRenderTarget(size.Width, size.Height, mipLevels, format);
	return LN_NEW Texture(Internal::Manager, obj, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::CreateDepthBuffer(const Size& size, TextureFormat format)
{
	LN_THROW(Internal::Manager != NULL, InvalidOperationException);
	Device::ITexture* obj = Helper::GetGraphicsDevice(Internal::Manager)->CreateDepthBuffer(size.Width, size.Height, format);
	return LN_NEW Texture(Internal::Manager, obj, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::CreateRenderTarget(GraphicsManager* manager, const Size& size, int mipLevels, TextureFormat format)
{
	LN_THROW(manager != NULL, ArgumentException);
	Device::ITexture* obj = Helper::GetGraphicsDevice(manager)->CreateRenderTarget(size.Width, size.Height, mipLevels, format);
	return LN_NEW Texture(manager, obj, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::CreateDepthBuffer(GraphicsManager* manager, const Size& size, TextureFormat format)
{
	LN_THROW(manager != NULL, ArgumentException);
	Device::ITexture* obj = Helper::GetGraphicsDevice(manager)->CreateDepthBuffer(size.Width, size.Height, format);
	return LN_NEW Texture(manager, obj, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture::Texture(GraphicsManager* manager, Device::ITexture* deviceObj, Imaging::Bitmap* primarySurface)
	: m_manager(manager)
	, m_deviceObj(deviceObj)
	, m_primarySurface(primarySurface)
	//, m_primarySurfaceModified(false)
{
	LN_SAFE_ADDREF(m_primarySurface);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture::~Texture()
{
	LN_SAFE_RELEASE(m_primarySurface);
	LN_SAFE_RELEASE(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Texture::GetSize() const
{
	return m_deviceObj->GetSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Texture::GetRealSize() const
{
	return m_deviceObj->GetRealSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureFormat Texture::GetFormat() const
{
	return m_deviceObj->GetTextureFormat();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::SetSubData(const Point& offset, Imaging::Bitmap* bitmap)
{
	if (LN_VERIFY_ASSERT(bitmap != NULL)) { return; }

	m_manager->GetRenderer()->m_primaryCommandList->AddCommand<Texture_SetSubDataBitmapCommand>(
		m_deviceObj, offset, bitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::SetSubData(const Point& offset, const void* data)
{
	if (LN_VERIFY_ASSERT(data != NULL)) { return; }
	// TODO: m_primarySurface にもセット
	m_manager->GetRenderer()->m_primaryCommandList->AddCommand<SetSubDataTextureCommand>(
		m_deviceObj, offset, data, m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Imaging::Bitmap* Texture::Lock()
{
	if (m_deviceObj->GetTextureType() == Device::TextureType_Normal)
	{
		return m_primarySurface;
	}
	else if (m_deviceObj->GetTextureType() == Device::TextureType_RenderTarget)
	{
		RenderingCommandList* cmdList = m_manager->GetRenderer()->m_primaryCommandList;
		cmdList->AddCommand<ReadLockTextureCommand>(this);

		// ここでたまっているコマンドをすべて実行する。
		// ReadLockTexture コマンドが実行されると、m_lockedBitmap に
		// ロックしたビットマップがセットされる。
		Helper::GetRenderingThread(m_manager)->PushRenderingCommand(cmdList);
		cmdList->WaitForIdle();

		return m_primarySurface;
	}

	LN_THROW(0, InvalidOperationException);
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::Unlock()
{
	RenderingCommandList* cmdList = m_manager->GetRenderer()->m_primaryCommandList;
	if (m_deviceObj->GetTextureType() == Device::TextureType_Normal)
	{
		// 変更済みフラグを ON にしておく。
		// あとで本当に使うタイミング (=シェーダパスのApply時) になったら FlushPrimarySurface() が呼ばれる
		//m_primarySurfaceModified = true;

		// TODO: 遅延転送
		//cmdList->SetTextureSubData(m_deviceObj, m_primarySurface);
		//cmdList->AddCommand<SetTextureSubDataCommand>(m_deviceObj, m_primarySurface);
		//SetTextureSubDataCommand::AddCommand(cmdList, m_deviceObj, m_primarySurface);
		cmdList->AddCommand<SetSubDataTextureCommand>(
			m_deviceObj, Point(0, 0), m_primarySurface->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
	}
	else if (m_deviceObj->GetTextureType() == Device::TextureType_RenderTarget)
	{
		cmdList->AddCommand<ReadUnlockTextureCommand>(this);
		//ReadUnlockTextureCommand::AddCommand(cmdList, this);
		//cmdList->ReadUnlockTexture(this);
		Helper::GetRenderingThread(m_manager)->PushRenderingCommand(cmdList);
		cmdList->WaitForIdle();
	}
}

} // namespace Graphics
} // namespace Lumino
