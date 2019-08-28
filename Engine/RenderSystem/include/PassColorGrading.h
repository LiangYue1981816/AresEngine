#pragma once
#include "RenderSystem.h"


class CALL_API CPassColorGrading : CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassColorGrading(CRenderSystem* pRenderSystem);
	virtual ~CPassColorGrading(void);


public:
	void SetFrameBuffer(CGfxRenderTexturePtr ptrColorGradingTexture);
	void SetInputColorTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorGradingTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
