#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionDefault : public CRenderSolutionBase
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
	virtual void Render(int indexQueue);
	virtual void Present(int indexQueue);


private:
	bool m_bEnableMSAA;

private:
	CGfxRenderTexturePtr m_ptrColorTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;
	CGfxFrameBufferPtr m_ptrFrameBufferScreen[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CGfxRenderPassPtr m_ptrRenderPassMSAA;
	CGfxRenderTexturePtr m_ptrColorTextureMSAA;
	CGfxRenderTexturePtr m_ptrDepthStencilTextureMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferScreenMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[2];
};
