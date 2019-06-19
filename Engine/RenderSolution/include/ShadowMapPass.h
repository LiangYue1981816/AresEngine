#pragma once
#include "PreHeader.h"


class CALL_API CRenderSolutionBase
{
	friend class CRenderSolution;


protected:
	CRenderSolutionBase(CRenderSolution* pRenderSolution);
	virtual ~CRenderSolutionBase(void);


protected:
	virtual void CreateDescriptorSet(void) = 0;
	virtual void DestroyDescriptorSet(void) = 0;

protected:
	virtual void CreateFrameBuffer(void) = 0;
	virtual void DestroyFrameBuffer(void) = 0;

	virtual void CreateFrameBufferMSAA(int samples) = 0;
	virtual void DestroyFrameBufferMSAA(void) = 0;

public:
	virtual void Create(int samples = 1) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual void UpdateCamera(int indexQueue) = 0;
	virtual void Render(int indexQueue) = 0;


protected:
	CRenderSolution* m_pRenderSolution;
};
