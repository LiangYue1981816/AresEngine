#pragma once
#include "RenderSystem.h"


class CALL_API CPassForwardLighting
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassForwardLighting(CCamera* pCamera, CRenderSystem* pRenderSystem);
	virtual ~CPassForwardLighting(void);


private:
	static void Create(const char* szName, GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat, int samples);
	static void Destroy(void);

public:
	void CreateFrameBuffer(int indexFrame, CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

public:
	void SetShadowTexture(CGfxRenderTexturePtr ptrShadowTexture);

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
