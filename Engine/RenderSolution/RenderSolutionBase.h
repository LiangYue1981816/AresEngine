#pragma once
#include "Engine.h"


class CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


protected:
	virtual void Render(void) = 0;
	virtual void Present(void) = 0;


protected:
	CTaskGraph m_taskCommandBuffer;
};
