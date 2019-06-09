#pragma once
#include "RenderSolutionBase.h"


class CALL_API CRenderSolutionTiledBaseForward : public CRenderSolutionBase
{
	friend class CRenderSolution;


protected:
	CRenderSolutionTiledBaseForward(CRenderSolution* pRenderSolution);
	virtual ~CRenderSolutionTiledBaseForward(void);


protected:
	virtual void CreateDescriptorSet(void);
	virtual void DestroyDescriptorSet(void);

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
