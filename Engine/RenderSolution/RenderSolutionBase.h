#pragma once
#include "Engine.h"


class CRenderSolutionBase
{
public:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


public:
	virtual void Render(void) = 0;
	virtual void Present(void) = 0;


protected:
	CTaskGraph m_taskCommandBuffer;
};
