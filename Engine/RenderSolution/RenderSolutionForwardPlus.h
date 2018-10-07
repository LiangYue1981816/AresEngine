#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionForwardPlus : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionForwardPlus(void);
	virtual ~CRenderSolutionForwardPlus(void);


private:
	virtual void Render(void);
	virtual void Present(void);
};
