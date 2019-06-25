#pragma once
#include "RenderSolution.h"


class CALL_API CPassForwardLighting
{
	friend class CRenderSolution;


private:
	CPassForwardLighting(CRenderSolution* pRenderSolution);
	virtual ~CPassForwardLighting(void);


private:
	const CGfxSemaphore* GetSemaphore(void) const;

private:
	void Update(void);
	void Render(int indexQueue, bool bMSAA, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxRenderPassPtr m_ptrRenderPassMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxDescriptorSetPtr m_ptrDescriptorSetLightingPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSolution* m_pRenderSolution;
};
