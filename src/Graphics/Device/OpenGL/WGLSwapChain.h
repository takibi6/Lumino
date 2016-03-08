﻿
#pragma once 
#include "GLSwapChain.h"
#include "GLGraphicsDevice.h"
#include "GLTexture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class WGLGraphicsDevice;
class WGLContext;

/// WGL 固有の ISwapChain の実装
class WGLSwapChain
	: public GLSwapChain
{
public:
	WGLSwapChain();
	virtual ~WGLSwapChain();

public:
	/// オブジェクト生成
	void Create(WGLGraphicsDevice* device, PlatformWindow* window, WGLContext* parentContext);


public:
	// override ISwapChain
	virtual ITexture* GetBackBuffer() { return m_renderTarget; }
	virtual void Resize(const Size& size);
	//virtual ITexture* GetBackBuffer();
	//virtual void Begin();
	//virtual void End();
	virtual void Present(ITexture* colorBuffer);

	// override GLSwapChain
	//virtual void MakeCurrentContext();	// これは削除予定

private:
	WGLGraphicsDevice*		m_device;
	PlatformWindow*			m_window;
	WGLContext*				m_context;
	GLRenderTargetTexture*	m_renderTarget;	///< 外部へ公開する見かけ上のレンダリングターゲット

	//HWND	m_hWnd;
	//HDC		m_hDC;
	//HGLRC	m_hGLRC;
	//GLBackbufferRenderTarget*	m_backBuffer;
};

class WGLContext
	: public GLContext
{
public:
	WGLContext(WGLGraphicsDevice* device, PlatformWindow* window, WGLContext* parentContext);
	virtual ~WGLContext();

	//virtual void MakeCurrentContext(bool attach);

	HDC GetDC() const { return m_hDC; }
	HGLRC GetGLRC() const { return m_hGLRC; }

private:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
