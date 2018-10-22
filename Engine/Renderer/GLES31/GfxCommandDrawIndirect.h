#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawIndirect : public CGfxCommandBase
{
public:
	CGfxCommandDrawIndirect(uint32_t mode, uint32_t type, uint32_t offset)
		: m_mode(mode)
		, m_type(type)
		, m_offset(offset)
	{

	}
	virtual ~CGfxCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			glDrawElementsIndirect(m_mode, m_type, (const void *)m_offset);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	uint32_t m_offset;
};
