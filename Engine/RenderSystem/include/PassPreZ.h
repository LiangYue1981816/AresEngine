#pragma once
#include "RenderSystem.h"


class CALL_API CPassPreZ
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat, int samples);
	static void Destroy(void);


private:
	CPassPreZ(CRenderSystem* pRenderSystem);
	virtual ~CPassPreZ(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetOutputTexture(CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
