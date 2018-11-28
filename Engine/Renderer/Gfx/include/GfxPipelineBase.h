#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineBase
{
public:
	CGfxPipelineBase(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxPipelineBase(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual bool IsTextureValid(uint32_t name) const = 0;
	virtual bool IsUniformValid(uint32_t name) const = 0;
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;


private:
	uint32_t m_name;
};
