#pragma once
#include "RenderSolutionBase.h"


class CRenderSolutionDefault : public CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionDefault(void);
	virtual ~CRenderSolutionDefault(void);


protected:
	virtual void CreateFrameBuffer(void);
	virtual void DestroyFrameBuffer(void);

	virtual void CreateFrameBufferMSAA(int samples);
	virtual void DestroyFrameBufferMSAA(void);

public:
	virtual void SetEnableMSAA(bool bEnable, int samples = 4);

public:
	virtual void Render(int indexQueue, float deltaTime);
	virtual void Present(int indexQueue);
	virtual void Clearup(int indexQueue);


private:
	bool m_bEnableMSAA;

private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxTexture2DPtr m_ptrDepthStencilTexture;
	CGfxFrameBufferPtr m_ptrFrameBufferScreen[CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT];

private:
	CGfxRenderPassPtr m_ptrRenderPassMSAA;
	CGfxTexture2DPtr m_ptrColorTextureMSAA;
	CGfxTexture2DPtr m_ptrDepthStencilTextureMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferScreenMSAA[CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT];

private:
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[2];
};
