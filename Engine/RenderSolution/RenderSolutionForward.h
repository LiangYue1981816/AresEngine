#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionForward : public CRenderSolutionBase
{
public:
	CRenderSolutionForward(void);
	virtual ~CRenderSolutionForward(void);


public:
	virtual void Render(void);
	virtual void Present(void);
};
