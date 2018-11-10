#pragma once
#include "GfxRenderer.h"


class CGfxPipelineCompute
{
public:
	CGfxPipelineCompute(void)
	{

	}
	virtual ~CGfxPipelineCompute(void)
	{

	}


public:
	virtual bool Create(const CGfxShader *pComputeShader) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual void Bind(void *pParam) = 0;
};
