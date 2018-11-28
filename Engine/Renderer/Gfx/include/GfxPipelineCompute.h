#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineCompute : public CGfxPipelineBase
{
public:
	CGfxPipelineCompute(uint32_t name)
		: CGfxPipelineBase(name)
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
