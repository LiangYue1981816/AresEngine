#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadowBlur
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassShadowBlur(CRenderSystem* pRenderSystem);
	virtual ~CPassShadowBlur(void);


private:
	static void Create(GfxPixelFormat shadowPixelFormat);
	static void Destroy(void);

public:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowBlurTexture);

public:
	void SetShadowTexture(CGfxRenderTexturePtr ptrShadowTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	CGfxCamera* m_pCamera;
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
