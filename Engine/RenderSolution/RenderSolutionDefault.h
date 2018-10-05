#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDefault : public CRenderSolutionBase
{
public:
	CRenderSolutionDefault(void);
	virtual ~CRenderSolutionDefault(void);


public:
	virtual void Render(void);
	virtual void Present(void);
};
