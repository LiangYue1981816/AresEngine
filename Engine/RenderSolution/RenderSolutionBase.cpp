#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_taskCommandBuffer("TashGraph_CommandBuffer", 50)
	, m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{
	m_ptrColorTextureMSAA = Renderer()->CreateTexture2D(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GL_RGBA, GL_RGBA8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, 4);

	m_ptrDepthStencilTextureMSAA = Renderer()->CreateTexture2D(HashValue("DepthStencilTextureMSAA"));
	m_ptrDepthStencilTextureMSAA->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, 4);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreens[index] = Renderer()->CreateFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(0, m_ptrDepthStencilTextureMSAA);
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(1, m_ptrColorTextureMSAA);
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(2, ptrColorTexture);
	}
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{

}
