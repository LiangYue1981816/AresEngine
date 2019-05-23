#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionDefault : public CRenderSolutionBase
{
	friend class CEngine;


private:
	typedef struct RenderParams {
		CGfxRenderPassPtr ptrRenderPass;
		CGfxFrameBufferPtr ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
		CGfxRenderTexturePtr ptrColorTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
		CGfxRenderTexturePtr ptrDepthStencilTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	} RenderParams;

	typedef struct RenderParamsMSAA {
		CGfxRenderPassPtr ptrRenderPass;
		CGfxFrameBufferPtr ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
		CGfxRenderTexturePtr ptrColorTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
		CGfxRenderTexturePtr ptrColorTexturesMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
		CGfxRenderTexturePtr ptrDepthStencilTexturesMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	} RenderParamsMSAA;


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
	virtual void Update(int indexQueue);
	virtual void Render(int indexQueue);


private:
	bool m_bEnableMSAA;
	RenderParams m_renderParams;
	RenderParamsMSAA m_renderParamsMSAA;

private:
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
