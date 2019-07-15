#pragma once
#include "RenderSolution.h"


class CALL_API CPassShadow
{
	friend class CCamera;
	friend class CRenderSolution;


private:
	CPassShadow(CCamera* pCamera, CRenderSolution* pRenderSolution);
	virtual ~CPassShadow(void);


private:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture);

private:
	void Update(void);
	void Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore);

private:
	const CGfxSemaphore* GetSemaphore(void) const;


private:
	float m_splitFactors[5];

private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	CGfxRenderTexturePtr m_ptrShadowTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSolution* m_pRenderSolution;
};
