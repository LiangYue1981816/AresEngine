#pragma once
#include "RenderSolution.h"


class CALL_API CPassShadow
{
	friend class CRenderSolution;


private:
	CPassShadow(CRenderSolution* pRenderSolution);
	virtual ~CPassShadow(void);


private:
	void Render(int indexQueue);


private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxDescriptorSetPtr m_ptrDescriptorSetShadowPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSolution* m_pRenderSolution;
};
