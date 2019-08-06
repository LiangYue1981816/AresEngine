#pragma once
#include "RenderSystem.h"


class CALL_API CPassDefault
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassDefault(CCamera* pCamera, CRenderSystem* pRenderSystem);
	virtual ~CPassDefault(void);


private:
	static void CreateRenderPass(const char* szName, GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat, int samples);
	static void DestroyRenderPass(void);

public:
	void CreateFrameBuffer(int indexFrame, CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, bool bPresent);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrColorTexture[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrDepthStencilTexture[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
