#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDefault : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionDefault(void);
	virtual ~CRenderSolutionDefault(void);


private:
	virtual void Render(void);
	virtual void Present(void);
};
