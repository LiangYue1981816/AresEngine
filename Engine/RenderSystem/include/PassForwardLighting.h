#pragma once
#include "RenderSystem.h"


class CALL_API CPassForwardLighting
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat, int samples);
	static void Destroy(void);


private:
	CPassForwardLighting(CRenderSystem* pRenderSystem);
	virtual ~CPassForwardLighting(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrShadowTexture);
	void SetOutputTexture(int indexFrame, CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore, int indexFrame, bool bPresent);


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
