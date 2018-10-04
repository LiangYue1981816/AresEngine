#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawElements : public CGfxCommandBase
{
public:
	CGfxCommandDrawElements(uint32_t mode, uint32_t type, int count, int baseIndex)
		: m_mode(mode)
		, m_type(type)
		, m_count(count)
		, m_baseIndex(baseIndex)
	{

	}
	virtual ~CGfxCommandDrawElements(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_ELEMENTS, "CommandDrawElements");
		{
			glDrawElements(m_mode, m_count, m_type, (const void *)m_baseIndex);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	int m_count;
	int m_baseIndex;
};
