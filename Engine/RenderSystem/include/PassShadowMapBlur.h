#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadowMapBlur
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassShadowMapBlur(CRenderSystem* pRenderSystem);
	virtual ~CPassShadowMapBlur(void);


private:
	static void Create(GfxPixelFormat shadowPixelFormat);
	static void Destroy(void);

public:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowBlurTexture);

public:
	void SetInputShadowTexture(CGfxRenderTexturePtr ptrShadowTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxRenderQueue* m_pRenderQueue;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrShadowBlurTexture;

private:
	CGfxUniformCamera* m_pCameraUniform;
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CRenderSystem* m_pRenderSystem;
};
