#pragma once
#include "RenderSolutionBase.h"


class CRenderSolutionDeferred : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionDeferred(void);
	virtual ~CRenderSolutionDeferred(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int samples = 4);

protected:
	virtual void CreateFrameBuffer(void);
	virtual void DestroyFrameBuffer(void);

	virtual void CreateFrameBufferMSAA(int samples);
	virtual void DestroyFrameBufferMSAA(void);

private:
	virtual void Render(int indexQueue);
	virtual void Present(int indexQueue);
	virtual void Clearup(int indexQueue);

private:
	virtual void RenderThread(void);
};
