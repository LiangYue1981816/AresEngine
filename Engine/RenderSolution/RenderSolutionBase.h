#pragma once
#include "Engine.h"


class CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int samples = 4) = 0;

protected:
	virtual void CreateFrameBuffer(void) = 0;
	virtual void DestroyFrameBuffer(void) = 0;

	virtual void CreateFrameBufferMSAA(int samples) = 0;
	virtual void DestroyFrameBufferMSAA(void) = 0;

protected:
	virtual void Render(int indexQueue) = 0;
	virtual void Present(int indexQueue) = 0;
	virtual void Clearup(int indexQueue) = 0;


protected:
	CTaskGraph m_taskCommandBuffer;
};
