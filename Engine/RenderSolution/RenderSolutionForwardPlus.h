#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionForwardPlus : public CRenderSolutionBase
{
public:
	CRenderSolutionForwardPlus(void);
	virtual ~CRenderSolutionForwardPlus(void);


public:
	virtual void Render(void);
	virtual void Present(void);
};
