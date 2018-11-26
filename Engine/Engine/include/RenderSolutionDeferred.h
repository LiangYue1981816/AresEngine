#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionDeferred : public CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionDeferred(void);
	virtual ~CRenderSolutionDeferred(void);


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
	virtual void Clearup(int indexQueue);
};
