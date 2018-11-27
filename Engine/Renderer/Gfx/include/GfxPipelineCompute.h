#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineCompute
{
protected:
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
	virtual bool IsTextureValid(uint32_t name) const = 0;
	virtual bool IsUniformValid(uint32_t name) const = 0;
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;

public:
	virtual void Bind(void *pParam) = 0;
};
