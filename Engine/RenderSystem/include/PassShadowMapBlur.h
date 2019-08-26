#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadowMapBlur
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat shadowPixelFormat);
	static void Destroy(void);


private:
	CPassShadowMapBlur(CRenderSystem* pRenderSystem);
	virtual ~CPassShadowMapBlur(void);


public:
	void SetFrameBuffer(CGfxRenderTexturePtr ptrShadowBlurTexture);
	void SetInputShadowMapTexture(CGfxRenderTexturePtr ptrShadowMapTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxRenderQueue* m_pRenderQueue;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrShadowMapBlurTexture;

private:
	CGfxUniformCamera* m_pCameraUniform;
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSystem* m_pRenderSystem;
};
