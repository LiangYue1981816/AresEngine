#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_taskCommandBuffer("TashGraph_CommandBuffer", 50)
	, m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{
	m_ptrDepthStencilTexture = Renderer()->CreateTexture2D(HashValue("DepthStencilTexture"));
	m_ptrDepthStencilTexture->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreens[index] = Renderer()->CreateFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(0, m_ptrDepthStencilTexture);
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(1, ptrColorTexture);
	}
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{

}
