#pragma once
#include "Engine.h"


class CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int width, int height, int samples = 4) = 0;

protected:
	virtual void Render(int indexQueue) = 0;
	virtual void Present(int indexQueue) = 0;
	virtual void Clearup(int indexQueue) = 0;


protected:
	CGfxCommandBuffer m_mainCommandBuffer[2];
	CGfxFrameBufferPtr m_ptrFrameBufferScreens[CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT];

protected:
	CTaskGraph m_taskCommandBuffer;
};
