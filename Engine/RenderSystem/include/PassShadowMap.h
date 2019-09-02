#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadowMap
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassShadowMap(CRenderSystem* pRenderSystem);
	virtual ~CPassShadowMap(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetFrameBuffer(CGfxRenderTexturePtr ptrDepthTexture);
	void SetSplitFactors(float f1, float f2, float f3, float f4);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	float m_splitFactors[5];
	CGfxCamera* m_pShadowCamera[4];
	CGfxRenderQueue* m_pShadowRenderQueue[4];

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrDepthTexture;

private:
	CGfxUniformCamera* m_pShadowCameraUniform[4];
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
