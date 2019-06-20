#pragma once
#include "RenderSolution.h"


class CALL_API CPassDefault
{
	friend class CRenderSolution;


private:
	CPassDefault(CRenderSolution* pRenderSolution);
	virtual ~CPassDefault(void);


private:
	void Render(int indexQueue, bool bMSAA);


private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxRenderPassPtr m_ptrRenderPassMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxDescriptorSetPtr m_ptrDescriptorSetDefaultPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSolution* m_pRenderSolution;
};
