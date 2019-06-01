#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionForwardPlus : public CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionForwardPlus(void);
	virtual ~CRenderSolutionForwardPlus(void);


protected:
	virtual void CreateFrameBuffer(void);
	virtual void DestroyFrameBuffer(void);

	virtual void CreateFrameBufferMSAA(int samples);
	virtual void DestroyFrameBufferMSAA(void);

public:
	virtual void Create(int samples = 1);
	virtual void Destroy(void);

public:
	virtual void Update(int indexQueue);
	virtual void Render(int indexQueue);
};
