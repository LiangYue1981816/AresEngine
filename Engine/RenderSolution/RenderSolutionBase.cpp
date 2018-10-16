#include "RenderSolutionBase.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_taskCommandBuffer("TashGraph_CommandBuffer", 50)
	, m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{
	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreens[index] = Renderer()->CreateFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreens[index]->SetAttachmentTexture(0, ptrColorTexture);
	}
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{

}
