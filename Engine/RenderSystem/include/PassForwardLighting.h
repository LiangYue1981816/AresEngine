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
	static void CreateRenderPass(const char* szName, GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat, int samples);
	static void DestroyRenderPass(void);

public:
	void CreateFrameBuffer(int indexFrame, CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Update(void);
	void Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);

private:
	const CGfxSemaphore* GetSemaphore(void) const;


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