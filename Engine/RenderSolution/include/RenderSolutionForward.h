#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionForward : public CRenderSolutionBase
{
	friend class CRenderSolution;


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
	CRenderSolutionForward(void);
	virtual ~CRenderSolutionForward(void);


protected:
	virtual void CreateFrameBuffer(void);
	virtual void DestroyFrameBuffer(void);

	virtual void CreateFrameBufferMSAA(int samples);
	virtual void DestroyFrameBufferMSAA(void);

public:
	virtual void Create(int samples = 1);
	virtual void Destroy(void);

public:
	virtual void UpdateCamera(int indexQueue);
	virtual void Render(int indexQueue);


private:
	bool m_bEnableMSAA;
	RenderParams m_renderParams;
	RenderParamsMSAA m_renderParamsMSAA;

private:
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
