#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionForward : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionForward(void);
	virtual ~CRenderSolutionForward(void);


private:
	virtual void Render(void);
	virtual void Present(void);
};
