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

private:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Update(void);
	void Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);

private:
	const CGfxSemaphore* GetSemaphore(void) const;


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorTexture;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
