#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDeferred : public CRenderSolutionBase
{
public:
	CRenderSolutionDeferred(void);
	virtual ~CRenderSolutionDeferred(void);


public:
	virtual void Render(void);
	virtual void Present(void);
};
