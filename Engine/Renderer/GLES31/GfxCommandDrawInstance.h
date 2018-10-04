#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawInstance : public CGfxCommandBase
{
public:
	CGfxCommandDrawInstance(uint32_t mode, uint32_t type, int count, int baseIndex, int instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_count(count)
		, m_baseIndex(baseIndex)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGfxCommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			glDrawElementsInstanced(m_mode, m_count, m_type, (const void *)m_baseIndex, m_instanceCount);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	int m_count;
	int m_baseIndex;
	int m_instanceCount;
};
