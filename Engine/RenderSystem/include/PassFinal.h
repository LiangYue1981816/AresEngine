#pragma once
#include "PreHeader.h"
#include "PassBlit.h"


class CALL_API CPassFinal : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassFinal(CRenderSystem* pRenderSystem);
	virtual ~CPassFinal(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);
	void SetOutputTexture(int indexFrame, CGfxRenderTexturePtr ptrColorTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, int indexFrame, bool bPresent);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputColorTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrOutputColorTexture[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
