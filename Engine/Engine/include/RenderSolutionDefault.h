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
	virtual void Update(int indexQueue);
	virtual void Render(int indexQueue);


private:
	bool m_bEnableMSAA;

private:
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrColorTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrDepthStencilTextures[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	/*
private:
	CGfxRenderPassPtr m_ptrRenderPassMSAA;
	CGfxRenderTexturePtr m_ptrColorTextureMSAA;
	CGfxRenderTexturePtr m_ptrDepthStencilTextureMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferScreenMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	*/

private:
	CGfxCommandBufferPtr m_ptrMainCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
