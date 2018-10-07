#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDeferred : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionDeferred(void);
	virtual ~CRenderSolutionDeferred(void);


private:
	virtual void Render(void);
	virtual void Present(void);
};
