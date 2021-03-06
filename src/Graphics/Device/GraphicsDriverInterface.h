﻿
#pragma once
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Color.h>

LN_NAMESPACE_BEGIN
class EngineDiagCore;
class Bitmap;
class PlatformWindow;

LN_NAMESPACE_GRAPHICS_BEGIN

namespace Driver
{
static const int MaxVertexStreams = 16;

class ISwapChain;
class IRenderer;
class IVertexDeclaration;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IShader;
class IShaderVariable;
class IShaderTechnique;
class IShaderPass;

/// デバイスの状態
enum DeviceState
{
	DeviceState_Enabled = 0,		///< 描画可能
	DeviceState_Lost,				///< 消失中。デバイスをリセットする必要がある。
	DeviceState_Pausing,			///< 
};

/// テクスチャサーフェイスの種類
enum TextureType
{
	TextureType_Normal = 0,					///< カラーテクスチャ
	TextureType_RenderTarget,				///< レンダリングターゲット
	TextureType_DepthBuffer,				///< 深度バッファ
	TextureType_BackBuffer,					///< バックバッファレンダリングターゲット
	TextureType_Max,						///< (terminator)
};



/// @brief	デバイスによって生成されるオブジェクト
class IDeviceObject
	: public RefObject
{
public:
	/// デバイス消失時の処理
	virtual void OnLostDevice() = 0;

	/// デバイス復帰時の処理
	virtual void OnResetDevice() = 0;

protected:
	virtual ~IDeviceObject() {}
};

/// グラフィックスデバイスのリソース管理を行う
class IGraphicsDevice
	: public RefObject
{
public:

	/// 終了処理
	virtual void Finalize() = 0;
	
	/// グラフィックスデバイスをライブラリ内部で生成したかを確認する
	virtual bool IsStandalone() const = 0;

	/// API 種類の取得
	virtual GraphicsAPI GetGraphicsAPI() const = 0;

	/// デフォルトのスワップチェインの取得
	virtual ISwapChain* GetDefaultSwapChain() = 0;
	
	/// 描画インターフェイスの取得
	virtual IRenderer* GetRenderer() = 0;

	virtual IVertexDeclaration* CreateVertexDeclaration(const VertexElement* elements, int elementsCount) = 0;

	/// 頂点バッファの作成
	virtual IVertexBuffer* CreateVertexBuffer(size_t bufferSize, const void* data, ResourceUsage usage) = 0;

	/// インデックスバッファの作成 (initialData を受け取るようにすれば、後で Lock 用にメモリ確保する必要がなくなる)
	virtual IIndexBuffer* CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) = 0;

	/// テクスチャの作成
	/// initialData : 初期データまたは NULL
	virtual ITexture* CreateTexture(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) = 0;

	/// テクスチャの作成 (環境依存の画像ファイル読み込み)
	virtual ITexture* CreateTexturePlatformLoading(Stream* stream, bool mipmap, TextureFormat format) = 0;

	virtual ITexture* CreateTexture3D(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) = 0;

	/// レンダーターゲットテクスチャの作成
	virtual ITexture* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) = 0;

	/// 深度バッファの作成
	virtual ITexture* CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format) = 0;

	/// シェーダの作成 (エラーの場合、インスタンスは作成されない)
	virtual IShader* CreateShader(const void* textData, size_t size, ShaderCompileResult* result) = 0;

	/// スワップチェインの作成
	virtual ISwapChain* CreateSwapChain(PlatformWindow* window) = 0;

	/// デバイスの状態を取得する
	virtual DeviceState GetDeviceState() = 0;

	/// デバイスをリセットする
	virtual void ResetDevice() = 0;

	/// デバイスを停止する
	virtual void OnLostDevice() = 0;

	/// デバイスを再開する
	virtual void OnResetDevice() = 0;

	/// 呼び出したスレッドを描画スレッドとして使用できるようにする (描画スレッドを使う場合のみ使用する)
	virtual void AttachRenderingThread() = 0;

	/// 描画スレッドの終了処理
	virtual void DetachRenderingThread() = 0;


	virtual void FlushResource() = 0;

protected:
	virtual ~IGraphicsDevice() {}
};

/// スワップチェインのインターフェイス
class ISwapChain
	: public IDeviceObject
{
public:

	/// バックバッファを示すレンダリングターゲット
	virtual ITexture* GetBackBuffer() = 0;

	/// バックバッファサイズを変更する
	virtual void Resize(const SizeI& size) = 0;

	/// バックバッファの内容をウィンドウへ転送する
	///		colorBuffer を本物のバックバッファへ一度コピーしてからウィンドウに Swap する。
	///		なぜこうしているのかというと、DX9 や OpenGL がデフォルトで作成するバックバッファは細かいフォーマットの制御が効かないため。
	///		さらに、OpenGL はバックバッファを Renderer::SetRenderTarget したいときはテクスチャ番号 0 をフレームバッファにアタッチする必要がある。
	///		これはカレントコンテキストに依存するため、複数ウィンドウへの描画を行いたいときに余計な制限をユーザに課すことになる。
	///		TODO: でも、今は colorBuffer は Graphics::Texture に持っているけど、Driver::SwapChain に持たせてもいいかも。
	virtual void Present(ITexture* colorBuffer) = 0;

protected:
	virtual ~ISwapChain() {}
};

/// 描画とそのステート管理を行うインターフェイス
class IRenderer
	: public RefObject
{
public:


	void EnterRenderState();
	void LeaveRenderState();

	virtual void Begin() = 0;
	virtual void End() = 0;

	void SetRenderState(const RenderState& state) { m_requestedRenderState = state; }
	const RenderState& GetRenderState() { return m_requestedRenderState; }
	virtual void SetDepthStencilState(const DepthStencilState& state) { m_requestedDepthStencilState = state; }
	const DepthStencilState& GetDepthStencilState() { return m_requestedDepthStencilState; }

	/// レンダリングターゲットの設定
	virtual void SetRenderTarget(int index, ITexture* texture) = 0;

	/// レンダリングターゲットの取得
	virtual ITexture* GetRenderTarget(int index) = 0;

	/// 深度バッファの設定
	virtual void SetDepthBuffer(ITexture* texture) = 0;

	/// 深度バッファの取得
	//virtual ITexture* GetDepthBuffer() = 0;

	/// ビューポート矩形の設定
	//virtual void SetViewport(const RectI& rect) = 0;

	/// ビューポート矩形の取得
	//virtual const RectI& GetViewport() = 0;

	void SetVertexDeclaration(IVertexDeclaration* vertexDeclaration);

	/// 頂点バッファの設定
	void SetVertexBuffer(int streamIndex, IVertexBuffer* vertexBuffer);

	/// インデックスバッファの設定
	void SetIndexBuffer(IIndexBuffer* indexBuffer);

	void SetShaderPass(IShaderPass* pass);

	/// 設定されている各種バッファをクリアする
	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

	/// プリミティブ描画
	void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);

	/// プリミティブ描画 (インデックス付き。頂点、インデックスの両方のバッファのdynamic、static が一致している必要がある)
	void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

	void SetDiag(EngineDiagCore* diag) { m_diag = diag; }

protected:
	IRenderer();
	virtual ~IRenderer();

	virtual void OnEnterRenderState() = 0;
	virtual void OnLeaveRenderState() = 0;
	virtual	void OnUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset) = 0;
	virtual	void OnUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset) = 0;
	virtual void OnUpdatePrimitiveData(IVertexDeclaration* decls, const List<RefPtr<IVertexBuffer>>& vertexBuufers, IIndexBuffer* indexBuffer) = 0;
	virtual void OnClear(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	virtual void OnDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) = 0;
	virtual void OnDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) = 0;

private:
	void FlushStates();

protected:	// TODO: private
	enum ModifiedFlags
	{
		Modified_None				= 0x0000,
		Modified_VertexDeclaration	= 0x0001,
		Modified_VertexBuffer		= 0x0002,
		Modified_IndexBuffer		= 0x0004,
		Modified_All				= 0xFFFFFFFF,
	};

	EngineDiagCore*					m_diag;
	uint32_t						m_modifiedFlags;
	RenderState						m_requestedRenderState;
	RenderState						m_currentRenderState;
	DepthStencilState				m_requestedDepthStencilState;
	DepthStencilState				m_currentDepthStencilState;
	RefPtr<IVertexDeclaration>		m_currentVertexDeclaration;
	List<RefPtr<IVertexBuffer>>		m_currentVertexBuffers;
	RefPtr<IIndexBuffer>			m_currentIndexBuffer;
	RefPtr<IShaderPass>				m_currentShaderPass;
};

/// 頂点宣言
class IVertexDeclaration
	: public IDeviceObject
{
protected:
	virtual ~IVertexDeclaration() = default;
};

/// 頂点バッファのインターフェイス
class IVertexBuffer
	: public IDeviceObject
{
public:

	/// 頂点データのバイト数の取得
	virtual size_t GetByteCount() const = 0;

	///// 頂点バッファリソースの使用方法の取得
	//virtual DeviceResourceUsage GetUsage() const = 0;

	/// データを転送する
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes) = 0;

	/// ロック
	virtual void* Lock() = 0;

	/// アンロック
	virtual void Unlock() = 0;

protected:
	virtual ~IVertexBuffer() {};
};

/// インデックスバッファのインターフェイス
class IIndexBuffer
	: public IDeviceObject
{
public:
	virtual size_t GetByteCount() const = 0;

	/// インデックスバッファのフォーマットの取得　
	virtual IndexBufferFormat GetFormat() const = 0;

	/// リソースの使用方法の取得
	virtual ResourceUsage GetUsage() const = 0;

	/// データを転送する
	virtual void SetSubData(uint32_t offsetBytes, const void* data, uint32_t dataBytes) = 0;

	/// ロック
	virtual void Lock(void** lockedBuffer, size_t* lockedSize) = 0;

	/// アンロック
	virtual void Unlock() = 0;

protected:
	virtual ~IIndexBuffer() {};
};

/// テクスチャのインターフェイス
class ITexture
	: public IDeviceObject
{
public:

	//void Clear(const Color& color);

	void SetSubData(const PointI& point, Bitmap* bitmap);



	/// テクスチャの種類の取得
	virtual TextureType GetTextureType() const = 0;

	/// サーフェイスフォーマットの取得
	virtual TextureFormat GetTextureFormat() const = 0;

	/// サイズの取得
    virtual const SizeI& GetSize() const = 0;

	/// 実際のサイズの取得 (デバイス依存により2のべき乗倍に拡張されたサイズ)
	virtual const SizeI& GetRealSize() const = 0;

	/// サンプラステートの設定
	virtual void SetSamplerState(const SamplerState& state) = 0;

	/// データ転送 (TODO:部分更新は未実装…)
	// data に渡されるイメージデータは上下が反転している状態。
	virtual void SetSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) = 0;

	virtual void SetSubData3D(const Box32& box, const void* data, size_t dataBytes) = 0;
	/// ロック
	virtual Bitmap* Lock() = 0;

	/// アンロック
	virtual void Unlock() = 0;

protected:
	virtual ~ITexture();

public:
	class ScopedLock
	{
	public:
		ScopedLock(ITexture* obj)
		{
			m_obj = obj;
			m_bitmap = m_obj->Lock();
		}
		~ScopedLock()
		{
			m_obj->Unlock();
		}
		Bitmap* GetBitmap()
		{
			return m_bitmap;
		}
	private:
		ITexture*			m_obj;
		Bitmap*	m_bitmap;
	};
};

//class ITexture2D
//	: public ITexture
//{
//public:
//
//protected:
//	virtual ~ITexture2D();
//};
//
//class ITexture3D
//	: public ITexture
//{
//public:
//
//protected:
//	virtual ~ITexture3D();
//};

/// シェーダプログラムのインターフェイス
class IShader
	: public IDeviceObject
{
public:

	/// 変数の数を取得する (GL は uniform 変数)
	virtual int GetVariableCount() const = 0;

	/// 変数を取得する
	virtual IShaderVariable* GetVariable(int index) const = 0;

	/// テクニックの数を取得する
	virtual int GetTechniqueCount() const = 0;

	/// テクニックを取得する
	virtual IShaderTechnique* GetTechnique(int index) const = 0;

public:

	/// 名前を指定して変数を取得する (見つからなければ NULL を返す)
	IShaderVariable* GetVariableByName(const TCHAR* name) const;

protected:
	virtual ~IShader() {}
};

/// シェーダプログラム内の変数のインターフェイス
/// (値の Get 系関数は、シェーダが持つメタ情報を取得するために使用する。
/// つまり、初期値の取得目的でしか使用できない)
class IShaderVariable
	: public RefObject
{
public:

	/// 変数の型の取得
	virtual ShaderVariableType GetType() const = 0;

	/// 変数名の取得
	virtual const String& GetName() const = 0;

	/// セマンティクス名の取得 (無い場合は NULL)
	virtual const String& GetSemanticName() const = 0;

	/// 行列型の場合の行の数を取得する
	virtual int GetMatrixRows() const = 0;

	/// 行列型の場合の列の数を取得する
	virtual int GetMatrixColumns() const = 0;

	/// 配列型の場合の要素数を取得する (0 の場合は配列ではない)
	virtual int GetArrayElements() const = 0;

	/// ブール値の設定
	virtual void SetBool(bool value) = 0;

	/// ブール値の取得
	//virtual bool GetBool() = 0;

	virtual void SetBoolArray(const bool* values, int count) = 0;

	/// 整数値の設定
	virtual void SetInt(int value) = 0;

	/// 整数値の取得
	//virtual int GetInt() = 0;

	/// 実数値の設定
	virtual void SetFloat(float value) = 0;

	/// 実数値の取得
	//virtual float GetFloat() = 0;

	virtual void SetFloatArray(const float* values, int count) = 0;

	/// ベクトルの設定
	virtual void SetVector(const Vector4& vec) = 0;

	/// ベクトルの取得
	//virtual const Vector4& GetVector() = 0;

	/// ベクトル配列の設定
	virtual void SetVectorArray(const Vector4* vectors, int count) = 0;

	/// ベクトル配列の取得
	//virtual const Vector4* GetVectorArray() = 0;

	/// 行列の設定
	virtual void SetMatrix(const Matrix& matrix) = 0;

	/// 行列の取得
	//virtual const Matrix& GetMatrix() = 0;

	/// 行列の配列の設定
	virtual void SetMatrixArray(const Matrix* matrices, int count) = 0;

	/// 行列の配列の取得
	//virtual const Matrix* GetMatrixArray() = 0;

	/// テクスチャの設定
	virtual void SetTexture(ITexture* texture) = 0;

	/// テクスチャの設定
	//virtual ITexture* GetTexture() = 0;

	/// 文字列の取得
	//virtual const TCHAR* GetString() = 0;

	virtual const ShaderValue& GetValue() const = 0;

	/// アノテーションの数を取得する
	virtual int GetAnnotationCount() = 0;

	/// アノテーションを取得する
	virtual IShaderVariable* GetAnnotation(int index) = 0;

protected:
	virtual ~IShaderVariable() {}
};

/// シェーダプログラム内のテクニックのインターフェイス
class IShaderTechnique
	: public RefObject
{
public:

	/// テクニックの名前を取得する
	virtual const TCHAR* GetName() const = 0;

	/// テクニック内のパスの数を取得する
	virtual int GetPassCount() const = 0;

	/// パスを取得する
	virtual IShaderPass* GetPass(int index) = 0;

	/// アノテーションの数を取得する
	virtual int GetAnnotationCount() = 0;

	/// アノテーションを取得する
	virtual IShaderVariable* GetAnnotation(int index) = 0;

protected:
	virtual ~IShaderTechnique() {}
};

/// シェーダプログラム内のパスのインターフェイス
class IShaderPass
	: public RefObject
{
public:

	/// パスの名前を取得する
	virtual const TCHAR* GetName() const = 0;

	/// アノテーションの数を取得する
	virtual int GetAnnotationCount() = 0;

	/// アノテーションを取得する
	virtual IShaderVariable* GetAnnotation(int index) = 0;

protected:
	friend class IRenderer;
	/// パスを適用する (CommitChanges するときも再コール)
	virtual void Apply() = 0;

	/// パスの適用を終了する
	//virtual void End() = 0;

protected:
	virtual ~IShaderPass() {}
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
