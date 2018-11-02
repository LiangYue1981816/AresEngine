#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawInstance : public CGfxCommandBase
{
public:
	CGfxCommandDrawInstance(uint32_t mode, uint32_t type, uint32_t offset, int count, int instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_offset(offset)
		, m_count(count)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGfxCommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			glDrawElementsInstanced(m_mode, m_count, m_type, (const void *)m_offset, m_instanceCount);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	uint32_t m_offset;
	int m_count;
	int m_instanceCount;
};
