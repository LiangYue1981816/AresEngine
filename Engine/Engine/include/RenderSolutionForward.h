#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionForward : public CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionForward(void);
	virtual ~CRenderSolutionForward(void);


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
};
