#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawElements : public CGfxCommandBase
{
public:
	CGLES3CommandDrawElements(uint32_t mode, uint32_t type, uint32_t offset, int count)
		: m_mode(mode)
		, m_type(type)
		, m_offset(offset)
		, m_count(count)
	{

	}
	virtual ~CGLES3CommandDrawElements(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_ELEMENTS, "CommandDrawElements");
		{
			glDrawElements(m_mode, m_count, m_type, (const void *)m_offset);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	uint32_t m_offset;
	int m_count;
};
