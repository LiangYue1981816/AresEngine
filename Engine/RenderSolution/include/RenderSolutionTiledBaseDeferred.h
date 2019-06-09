#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionTiledBaseDeferred : public CRenderSolutionBase
{
	friend class CRenderSolution;


protected:
	CRenderSolutionTiledBaseDeferred(CRenderSolution* pRenderSolution);
	virtual ~CRenderSolutionTiledBaseDeferred(void);


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
};
