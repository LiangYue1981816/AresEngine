#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineCompute
{
public:
	CGfxPipelineCompute(uint32_t name)
	{

	}
	virtual ~CGfxPipelineCompute(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual HANDLE GetPipeline(void) const = 0;

public:
	virtual bool Create(const CGfxShader *pComputeShader) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsTextureValid(uint32_t name) const = 0;
	virtual bool IsUniformValid(uint32_t name) const = 0;
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;
};
