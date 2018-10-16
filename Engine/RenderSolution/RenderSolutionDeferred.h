#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDeferred : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionDeferred(void);
	virtual ~CRenderSolutionDeferred(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int width, int height, int samples = 4);

private:
	virtual void Render(int indexQueue);
	virtual void Present(int indexQueue);
	virtual void Clearup(int indexQueue);
};
