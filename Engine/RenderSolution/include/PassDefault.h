#pragma once
#include "RenderSolution.h"


class CALL_API CPassDefault
{
	friend class CCamera;
	friend class CRenderSolution;


private:
	CPassDefault(CCamera* pCamera, CRenderSolution* pRenderSolution);
	virtual ~CPassDefault(void);


private:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore);
	const CGfxSemaphore* GetSemaphore(void) const;


private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	CGfxRenderTexturePtr m_ptrColorTexture;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSolution* m_pRenderSolution;
};
