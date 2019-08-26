#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadowMap
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassShadowMap(CCamera* pCamera, CRenderSystem* pRenderSystem);
	virtual ~CPassShadowMap(void);


private:
	static void Create(GfxPixelFormat shadowPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);

public:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	float m_splitFactors[5];
	CGfxCamera* m_pShadowCamera[4];
	CGfxRenderQueue* m_pShadowRenderQueue[4];

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrShadowTexture;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;

private:
	CGfxUniformCamera* m_pShadowCameraUniform[4];
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
