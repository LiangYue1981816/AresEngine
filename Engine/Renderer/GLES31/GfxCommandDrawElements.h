#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawElements : public CGfxCommandBase
{
public:
	CGfxCommandDrawElements(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex)
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
	GLenum m_mode;
	GLenum m_type;
	GLsizei m_count;
	GLsizei m_baseIndex;
};
