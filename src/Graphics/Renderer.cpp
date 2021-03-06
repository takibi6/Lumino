﻿
#include "../Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Shader.h>
#include "RendererImpl.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Renderer
//==============================================================================

//------------------------------------------------------------------------------
static Details::Renderer* GetRenderer()
{
	return detail::GraphicsManager::GetInstance()->GetRenderer();
}


//==============================================================================
// Details::Renderer
//==============================================================================

LN_NAMESPACE_GRAPHICS_BEGIN
namespace Details
{

//------------------------------------------------------------------------------
Renderer::Renderer(detail::GraphicsManager* manager)
	: m_internal(manager->GetGraphicsDevice()->GetRenderer())
	, m_primaryCommandList(NULL)
	, m_currentRenderState()
	, m_currentDepthStencilState()
	, m_currentDepthBuffer(NULL)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
	GraphicsResourceObject::Initialize(manager);

	//if (m_manager->GetRenderingType() == GraphicsRenderingType::Deferred) {
	m_primaryCommandList = LN_NEW RenderingCommandList(manager);
	//}
}

//------------------------------------------------------------------------------
Renderer::~Renderer()
{
	if (m_primaryCommandList != NULL)
	{
		m_primaryCommandList->PostExecute();	// Present される前に解放されることの対策
		LN_SAFE_RELEASE(m_primaryCommandList);
	}

	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
	LN_SAFE_RELEASE(m_currentDepthBuffer);
}

//------------------------------------------------------------------------------
void Renderer::Begin()
{
	bool isStandalone = m_manager->GetGraphicsDevice()->IsStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		Begin, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->Begin();
				m_internal->EnterRenderState();
			}
			else
			{
				m_internal->EnterRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void Renderer::End()
{
	bool isStandalone = m_manager->GetGraphicsDevice()->IsStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		End, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->LeaveRenderState();
				m_internal->End();
			}
			else
			{
				m_internal->LeaveRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void Renderer::SetRenderState(const RenderState& state)
{
	m_currentRenderState = state;

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetRenderState, m_manager,
		Driver::IRenderer*, m_internal,
		RenderState, state,
		{
			m_internal->SetRenderState(state);
		});
}

//------------------------------------------------------------------------------
const RenderState& Renderer::GetRenderState() const
{
	return m_currentRenderState;
}

//------------------------------------------------------------------------------
void Renderer::SetDepthStencilState(const DepthStencilState& state)
{
	m_currentDepthStencilState = state;

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetDepthStencilState, m_manager,
		Driver::IRenderer*, m_internal,
		DepthStencilState, state,
		{
			m_internal->SetDepthStencilState(state);
		});
}

//------------------------------------------------------------------------------
const DepthStencilState& Renderer::GetDepthStencilState() const
{
	return m_currentDepthStencilState;
}

//------------------------------------------------------------------------------
void Renderer::SetRenderTarget(int index, Texture* texture)
{
	Driver::ITexture* t = (texture != NULL) ? texture->ResolveDeviceObject() : NULL;
	LN_REFOBJ_SET(m_currentRenderTargets[index], texture);

	LN_ENQUEUE_RENDER_COMMAND_3(
		SetRenderTarget, m_manager,
		Driver::IRenderer*, m_internal,
		int, index,
		RefPtr<Driver::ITexture>, t,
		{
			m_internal->SetRenderTarget(index, t);
		});
}

//------------------------------------------------------------------------------
Texture* Renderer::GetRenderTarget(int index) const
{
	LN_THROW(0 <= index && index < Graphics::MaxMultiRenderTargets, ArgumentException);
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void Renderer::SetDepthBuffer(DepthBuffer* depthBuffer)
{
	Driver::ITexture* t = (depthBuffer != nullptr) ? depthBuffer->ResolveDeviceObject() : nullptr;
	LN_REFOBJ_SET(m_currentDepthBuffer, depthBuffer);

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetDepthBuffer, m_manager,
		Driver::IRenderer*, m_internal,
		RefPtr<Driver::ITexture>, t,
		{
			m_internal->SetDepthBuffer(t);
		});
}

//------------------------------------------------------------------------------
DepthBuffer* Renderer::GetDepthBuffer() const
{
	return m_currentDepthBuffer;
}

//------------------------------------------------------------------------------
//void Renderer::SetViewport(const RectI& rect)
//{
//	//LN_CALL_RENDERER_COMMAND(SetViewport, SetViewportCommand, rect);
//	m_currentViewport = rect;
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetViewport, m_manager,
//		Driver::IRenderer*, m_internal,
//		RectI, rect,
//		{
//			m_internal->SetViewport(rect);
//		});
//}

////------------------------------------------------------------------------------
//const RectI& Renderer::GetViewport()
//{
//	return m_currentViewport;
//}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Renderer::SetVertexBuffer(VertexBuffer* vertexBuffer)
//{
//	Driver::IVertexBuffer* t = (vertexBuffer != NULL) ? Helper::GetDeviceObject(vertexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(SetVertexBuffer, SetVertexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetVertexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IVertexBuffer>, t,
//		{
//			m_internal->SetVertexBuffer(t);
//		});
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
//{
//	Driver::IIndexBuffer* t = (indexBuffer != NULL) ? Helper::GetDeviceObject(indexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(SetIndexBuffer, SetIndexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetIndexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IIndexBuffer>, t,
//		{
//			m_internal->SetIndexBuffer(t);
//		});
//}

//------------------------------------------------------------------------------
void Renderer::SetShaderPass(ShaderPass* pass)
{
	if (pass != nullptr)
		pass->Apply();
}

//------------------------------------------------------------------------------
void Renderer::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	VerifyFrameBuffers();

	LN_ENQUEUE_RENDER_COMMAND_5(
		Clear, m_manager,
		Driver::IRenderer*, m_internal,
		ClearFlags, flags,
		Color, color, 
		float, z, 
		uint8_t, stencil,
		{
			m_internal->Clear(flags, color, z, stencil);
		});

	//LN_CALL_RENDERER_COMMAND(Clear, ClearCommand, flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void Renderer::DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	VerifyFrameBuffers();

	Driver::IVertexDeclaration* decl = (vertexDeclaration != nullptr) ? vertexDeclaration->GetDeviceObject() : nullptr;
	Driver::IVertexBuffer* vb = (vertexBuffer != nullptr) ? vertexBuffer->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_6(
		DrawPrimitive, m_manager,
		Driver::IRenderer*, m_internal,
		Driver::IVertexDeclaration*, decl,
		Driver::IVertexBuffer*, vb,
		PrimitiveType, primitive,
		int, startVertex,
		int, primitiveCount,
		{
			m_internal->SetVertexDeclaration(decl);
			m_internal->SetVertexBuffer(0, vb);
			m_internal->DrawPrimitive(primitive, startVertex, primitiveCount);
		});

	//LN_CALL_RENDERER_COMMAND(DrawPrimitive, DrawPrimitiveCommand, primitive, startVertex, primitiveCount);
}

//------------------------------------------------------------------------------
void Renderer::DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	VerifyFrameBuffers();

	Driver::IVertexDeclaration* decl = (vertexDeclaration != nullptr) ? vertexDeclaration->GetDeviceObject() : nullptr;
	Driver::IVertexBuffer* vb = (vertexBuffer != nullptr) ? vertexBuffer->GetDeviceObject() : nullptr;
	Driver::IIndexBuffer* ib = (indexBuffer != nullptr) ? indexBuffer->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		DrawPrimitiveIndexed, m_manager,
		Driver::IRenderer*, m_internal,
		Driver::IVertexDeclaration*, decl,
		Driver::IVertexBuffer*, vb,
		Driver::IIndexBuffer*, ib,
		PrimitiveType, primitive,
		int, startIndex,
		int, primitiveCount,
		{
			m_internal->SetVertexDeclaration(decl);
			m_internal->SetVertexBuffer(0, vb);
			m_internal->SetIndexBuffer(ib);
			m_internal->DrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
		});
	//LN_CALL_RENDERER_COMMAND(DrawPrimitiveIndexed, DrawPrimitiveIndexedCommand, primitive, startIndex, primitiveCount);
}

//------------------------------------------------------------------------------
void Renderer::FlushState(const detail::ContextState& state)
{
	// TODO: 1つのコマンドで一括設定したい
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::CommonState))
	{
		SetRenderState(state.renderState);
		SetDepthStencilState(state.depthStencilState);
		for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i) {
			SetRenderTarget(i, state.GetRenderTarget(i));
		}
		SetDepthBuffer(state.depthBuffer);
		//SetViewport(state.viewport);
		//SetVertexBuffer(state.vertexBuffer);
		//SetIndexBuffer(state.indexBuffer);
	}
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::ShaderPass))
	{
		if (state.GetShaderPass() != nullptr)
		{
			state.GetShaderPass()->Apply();
		}
	}
}

//------------------------------------------------------------------------------
void Renderer::Flush()
{

}

//------------------------------------------------------------------------------
void Renderer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
	}
	else {
		m_internal = device->GetRenderer();
	}
}

//------------------------------------------------------------------------------
void Renderer::PresentCommandList(SwapChain* swapChain)
{

	m_primaryCommandList->AddCommand<PresentCommand>(swapChain);
	
	auto* renderingThread = m_manager->GetRenderingThread();
	renderingThread->PushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	RenderingCommandList* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;
}

//------------------------------------------------------------------------------
void Renderer::VerifyFrameBuffers()
{
	// レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	if (m_currentDepthBuffer != nullptr)
	{
		LN_CHECK_STATE(m_currentRenderTargets[0]->GetSize() == m_currentDepthBuffer->GetSize());
	}
}

} // namespace Details
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
