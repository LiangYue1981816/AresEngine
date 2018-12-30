#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineCompute : public CGfxPipeline
{
public:
	CGfxPipelineCompute(uint32_t name)
		: CGfxPipeline(name)
	{

	}
	virtual ~CGfxPipelineCompute(void)
	{

	}


public:
	virtual bool Create(const CGfxShader *pComputeShader) = 0;
	virtual void Destroy(void) = 0;
};
