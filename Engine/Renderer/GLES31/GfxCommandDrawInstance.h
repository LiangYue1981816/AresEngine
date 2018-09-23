#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawInstance : public CGfxCommandBase
{
public:
	CGfxCommandDrawInstance(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei instanceCount)
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
	GLenum m_mode;
	GLenum m_type;
	GLsizei m_count;
	GLsizei m_baseIndex;
	GLsizei m_instanceCount;
};
