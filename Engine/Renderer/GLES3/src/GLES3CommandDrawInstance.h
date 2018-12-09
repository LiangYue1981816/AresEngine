#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(GfxDrawMode mode, GfxIndexType type, uint32_t offset, int count, int instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_offset(offset)
		, m_count(count)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			glDrawElementsInstanced(GLDrawMode(m_mode), m_count, GLIndexType(m_type), (const void *)m_offset, m_instanceCount);
		}
	}


private:
	GfxDrawMode m_mode;
	GfxIndexType m_type;
	uintptr_t m_offset;
	int m_count;
	int m_instanceCount;
};
