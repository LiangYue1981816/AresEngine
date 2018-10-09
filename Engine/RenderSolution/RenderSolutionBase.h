#pragma once
#include "Engine.h"


class CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(int screenWidth, int screenHeight);
	virtual ~CRenderSolutionBase(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int width, int height, int samples = 4) = 0;

protected:
	virtual void Render(int indexQueue) = 0;
	virtual void Present(int indexQueue) = 0;
	virtual void Clearup(int indexQueue) = 0;


protected:
	CGfxCommandBuffer m_mainCommandBuffer[2];
	CGfxFrameBufferPtr m_ptrFrameBufferScreen;

protected:
	CTaskGraph m_taskCommandBuffer;
};
