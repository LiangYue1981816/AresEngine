#pragma once
#include "RenderSystem.h"


class CALL_API CPassBlend : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassBlend(CRenderSystem* pRenderSystem);
	virtual ~CPassBlend(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTextureSrc, CGfxRenderTexturePtr ptrColorTextureDst);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
