#pragma once
#include "RenderSolution.h"


class CALL_API CPassShadow
{
	friend class CRenderSolution;


private:
	CPassShadow(CRenderSolution* pRenderSolution);
	virtual ~CPassShadow(void);


private:
	const CGfxSemaphore* GetSemaphore(void) const;

private:
	void SetSplitFactor(float f1, float f2, float f3);

private:
	void Update(void);
	void Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore);


private:
	float m_splitFactors[5];

private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	CGfxDescriptorSetPtr m_ptrDescriptorSetShadowPass[4];
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSolution* m_pRenderSolution;
};
