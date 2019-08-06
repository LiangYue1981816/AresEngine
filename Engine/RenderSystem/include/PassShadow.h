#pragma once
#include "RenderSystem.h"


class CALL_API CPassShadow
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	CPassShadow(CCamera* pCamera, CRenderSystem* pRenderSystem);
	virtual ~CPassShadow(void);


private:
	static void CreateRenderPass(const char* szName, GfxPixelFormat shadowPixelFormat);
	static void DestroyRenderPass(void);

public:
	void CreateFrameBuffer(CGfxRenderTexturePtr ptrShadowTexture);

private:
	const CGfxSemaphore* Render(CTaskGraph& taskGraph, const CGfxSemaphore* pWaitSemaphore);


private:
	float m_splitFactors[5];

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrShadowTexture;

private:
	CGfxUniformCamera* m_pShadowCameraUniform[4];
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
