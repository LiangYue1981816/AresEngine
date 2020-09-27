#pragma once
#include "PreHeader.h"


class CGfxPipelineCompute
{
public:
	CGfxPipelineCompute(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxPipelineCompute(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const = 0;


private:
	uint32_t m_name;
};
