#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxPipelineBase
{
protected:
	CGfxPipelineBase(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxPipelineBase(void)
	{

	}

public:
	uint32_t GetName(void) const
	{
		return m_name;
	}


private:
	uint32_t m_name;
};
