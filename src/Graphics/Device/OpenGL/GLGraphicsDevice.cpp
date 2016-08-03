﻿/*
 *	EXT とか ARB とか
 *	https://shikihuiku.wordpress.com/2013/10/08/hardtounderstandoglext/
 */
#include "../../../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include <Lumino/Platform/PlatformWindow.h>
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLGraphicsDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// GLGraphicsDevice
//==============================================================================

//------------------------------------------------------------------------------
GLGraphicsDevice::GLGraphicsDevice()
	: m_deviceState(DeviceState_Lost)
	, m_renderer(NULL)
	, m_openGLMajorVersion(3)
	, m_openGLMinorVersion(1)
{
}

//------------------------------------------------------------------------------
GLGraphicsDevice::~GLGraphicsDevice()
{
	LN_ASSERT(m_renderer == NULL);	// Finalize 済みであること
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::Initialize(const ConfigData& configData)
{
	m_mainWindow = configData.MainWindow;
	m_deviceState = DeviceState_Enabled;


	Logger::WriteLine("GLGraphicsDevice::Initialize");
	Logger::WriteLine("    Requested OpenGL version : %d.%d", configData.OpenGLMajorVersion, configData.OpenGLMinorVersion);
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::Finalize()	// 仮想関数呼び出しが必要なのでデストラクタとは別に開放用関数を用意した
{
	OnBeginAccessContext();
	GraphicsDeviceBase::Finalize();
	LN_SAFE_RELEASE(m_renderer);
	OnEndAccessContext();

	//ScopedContext lock(this);
	//LN_FOREACH(IDeviceObject* obj, m_allDeviceResourceList) {
	//	obj->Release();
	//}
	//m_allDeviceResourceList.Clear();
}

//------------------------------------------------------------------------------
RefPtr<IVertexDeclaration> GLGraphicsDevice::CreateVertexDeclarationImplement(const VertexElement* elements, int elementsCount)
{
	RefPtr<GLVertexDeclaration> obj(LN_NEW GLVertexDeclaration(), false);
	obj->Initialize(elements, elementsCount);
	return RefPtr<IVertexDeclaration>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<IVertexBuffer> GLGraphicsDevice::CreateVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage)
{
	RefPtr<GLVertexBuffer> obj(LN_NEW GLVertexBuffer(), false);
	obj->Create(bufferSize, data, usage);
    return RefPtr<IVertexBuffer>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<IIndexBuffer> GLGraphicsDevice::CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	RefPtr<GLIndexBuffer> obj(LN_NEW GLIndexBuffer(), false);
    obj->Create(indexCount, initialData, format, usage);
    return RefPtr<IIndexBuffer>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateTextureImplement(const SizeI& size, uint32_t mipLevels, TextureFormat format, const void* initialData)
{
	RefPtr<GLTexture> obj(LN_NEW GLTexture(size, format, mipLevels), false);
	if (initialData != nullptr) {
		obj->SetSubData(Point(0, 0), initialData, Utils::GetTextureFormatByteCount(format) * size.width * size.height, size);
    }
    return RefPtr<ITexture>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	RefPtr<GLRenderTargetTexture> obj(LN_NEW GLRenderTargetTexture(SizeI(width, height), format, mipLevels), false);
    return RefPtr<ITexture>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format)
{
	RefPtr<GLDepthBuffer> obj(LN_NEW GLDepthBuffer(SizeI(width, height), format), false);
    return RefPtr<ITexture>::StaticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<IShader> GLGraphicsDevice::CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result)
{
	GLShader* shader = LN_NEW GLShader();
	shader->Initialize(this, textData, size);
	result->Level = shader->GetDiag()->level;
	result->Message = shader->GetDiag()->message;

	//printf("▼\n");

	//GLShader* shader = NULL;
	//result->Level = GLSLUtils::Build(this, textData, size, &shader, &result->Message);



	//if (shader != NULL) {
	//	AddDeviceResource(shader);
	//}
	/*
		シェーダを作った直後、Shader の Apply() → DrawPrimitive() → Present() すると、
		glFlush() とか wglMakeCurrent() とかでビジー状態になり、「ディスプレイドライバが応答しません」とか右下からエラー出た。
		リソースをメインスレッドで作って、描画スレッドで ドライバに描画情報送るときに落ちたと思われるが、ホントの原因は不明。
		SwapChain を 2 つ作ったりすると何故か正常に動作した。

		http://stackoverflow.com/questions/21613842/wglmakecurrent-in-second-thread-fails
		によると、グラボによってはバグを回避するために wglMakeCurrent(NULL, NULL) を仕込む必要があるとのこと。
		とりあえずそのようにしてみたところ、うまく動作するようになった。
		問題のグラボは AMD Radeon(TM) HD8490。

		GeForce GTX 560M では正常に動作した。
	*/
	//MakeCurrentContext(NULL);
	//MakeCurrentContext(GetMainContext());

	//printf("▲\n");
	RefPtr<IShader> obj(shader, false);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ISwapChain> GLGraphicsDevice::CreateSwapChainImplement(PlatformWindow* window)
{
	LN_THROW(0, NotImplementedException);
	return nullptr;
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::ResetDevice()
{
	// 先に OnLostDevice() を呼ぶこと
	LN_THROW(m_deviceState == DeviceState_Pausing, InvalidOperationException);
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::OnLostDevice()
{
	m_deviceState = DeviceState_Pausing;
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::OnResetDevice()
{
	m_deviceState = DeviceState_Enabled;
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::LockContext()
{
	m_mutex.Lock();
	OnBeginAccessContext();
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::UnlockContext()
{
	OnEndAccessContext();
	m_mutex.Unlock();
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::ParseGLVersion(int* glMajor, int* glMinor, int* glslMajor, int* glslMinor)
{
	// GL_VERSION の文字列フォーマットは決まっている。
	// https://www.opengl.org/wiki/GLAPI/glGetString
	// 現在は 1桁バージョンしかないので、Haskell の GLHUI はこんな単純なとり方をしていたりする。
	// http://hackage.haskell.org/package/GLHUI-1.1.0/src/c/HsGLWindow.c
	const char* version = (const char*)glGetString(GL_VERSION);
	*glMajor = version[0] - '0';
	*glMinor = version[2] - '0';
	if ((*glMajor) >= 2) {
		version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		*glslMajor = version[0] - '0';
		*glslMinor = version[2] - '0';
	}
	else {
		*glslMajor = 0;
		*glslMinor = 0;
	}
}

//------------------------------------------------------------------------------
bool GLGraphicsDevice::ContainsExtensionString(const char* extensionString, const char* str)
{
	const char* readPos = extensionString;
	const char* extensionStringEnd = extensionString + strlen(extensionString);
	while (true)
	{
		const char* found = strstr(readPos, str);
		if (found == NULL) {
			return false;
		}

		// 見つかったところは、ホントにトークンの先頭を指しているか
		if (found == readPos || *(found - 1) == ' ')
		{
			const char* endPos = found + strlen(str);
			if (endPos <= extensionStringEnd)
			{
				// 見つかった範囲の終端は次のトークンとの区切りのスペース文字か、\0
				if (*endPos == ' ' || *endPos == '\0') {
					break;	// 見つかった
				}
			}
		}
	}

	// break で抜けてきて、true を返す
	return true;
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::SelectGLVersion(int requestMajor, int requestMinor)
{
	if (requestMajor == 0)
	{
		// 要求されていない場合は環境のデフォルトバージョンでコンテキストを作成する
		m_openGLMajorVersion = 0;
		m_openGLMinorVersion = 0;
	}
	else
	{
		// OpenGL バージョンの選択 (最大でも GL_VERSION にする)
		int glMajor, glMinor, glslMajor, glslMinor;
		ParseGLVersion(&glMajor, &glMinor, &glslMajor, &glslMinor);
		if (requestMajor > glMajor) {
			m_openGLMajorVersion = glMajor;
			m_openGLMinorVersion = glMinor;
		}
		else if (requestMinor > glMinor) {
			m_openGLMajorVersion = requestMajor;
			m_openGLMinorVersion = glMinor;
		}
		else {
			m_openGLMajorVersion = requestMajor;
			m_openGLMinorVersion = requestMinor;
		}
	}

	Logger::WriteLine("Active OpenGL version : %d.%d", m_openGLMajorVersion, m_openGLMinorVersion);
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::AttachRenderingThread()
{
	MakeCurrentContext(GetMainRenderingContext());
	GraphicsDeviceBase::AttachRenderingThread();
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::DetachRenderingThread()
{
	MakeCurrentContext(nullptr);
	GraphicsDeviceBase::DetachRenderingThread();
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::OnBeginAccessContext()
{
	if (Thread::GetCurrentThreadID() != m_attachRenderingThreadId)
	{
		MakeCurrentContext(GetMainContext());
	}
}

//------------------------------------------------------------------------------
void GLGraphicsDevice::OnEndAccessContext()
{
	if (Thread::GetCurrentThreadID() != m_attachRenderingThreadId)
	{
		MakeCurrentContext(nullptr);
	}
}

//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GLGraphicsDevice::AddDeviceResource(IDeviceObject* obj)
//{
//	MutexScopedLock lock(m_allDeviceResourceListMutex);
//
//	m_allDeviceResourceList.Add(obj);
//	obj->AddRef();
//}
//
////------------------------------------------------------------------------------
//// 
////------------------------------------------------------------------------------
//void GLGraphicsDevice::GCDeviceResource()
//{
//	/*
//		この関数は描画スレッドの、SwapChain::Present() の直後で実行される。
//		リソースがこのクラス以外から参照されていなければ開放する。
//
//		なお、このような仕組みにしたのは、リソースの開放を容易にするため。
//		Create 系はスレッドセーフかつ GPU 固有の不具合に備えるため、
//		作成の前後でコンテキストをアクティブ/ディアクティブしているが、
//		開放時 (glDelete～) の前後でも当然必要になる。
//
//		遅延描画の都合上、メインスレッドで不要になっても描画スレッドではまだ使っていることは普通にある。
//		描画スレッドでも必要なくなった時点でリソースを Release すれば良いのだが、
//		それだとデストラクタで「現在のスレッドがメインスレッドであれば MakeCurrent する」のような処理が必要になる。
//		この場合問題となるのは、
//		・Create系の内部でエラーが発生し、Release したいときにデッドロックの危険性がある
//		・デストラクタで例外を発生させる可能性がある
//		やりようはいくらでもあるが、シンプルに実装するのは少し難しい。
//	*/
//	MutexScopedLock lock(m_allDeviceResourceListMutex);
//
//	Array<IDeviceObject*>::iterator itr = m_allDeviceResourceList.begin();
//	Array<IDeviceObject*>::iterator end = m_allDeviceResourceList.end();
//	for (; itr != end;)
//	{
//		if ((*itr)->GetReferenceCount() == 1) {
//			(*itr)->Release();
//			itr = m_allDeviceResourceList.erase(itr);
//			end = m_allDeviceResourceList.end();
//		}
//		else {
//			++itr;
//		}
//	}
//	
//
//	//LN_FOREACH(IDeviceObject* obj, m_allDeviceResourceList)
//	//{
//	//	if (obj->GetReferenceCount() == 1) {
//	//		obj->Release();
//	//	}
//	//}
//}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
