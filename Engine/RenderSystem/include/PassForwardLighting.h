#pragma once
#include "RenderSystem.h"


class CALL_API CPassForwardLighting
{
	friend class CCamera;
	friend class CRenderSolution;


private:
	CPassForwardLighting(CCamera* pCamera, CRenderSolution* pRenderSolution);
	virtual ~CPassForwardLighting(void);


private:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Update(void);
	void Render(int indexQueue, const CGfxSemaphore* pWaitSemaphore);

private:
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
