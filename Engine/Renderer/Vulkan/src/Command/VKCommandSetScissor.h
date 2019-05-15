#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetScissor : public CGfxCommandBase
{
public:
	CGLES3CommandSetScissor(int x, int y, int width, int height)
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{

	}
	virtual ~CGLES3CommandSetScissor(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_SCISSOR, "CommandSetScissor");
		{
			GLEnable(GL_SCISSOR_TEST);
			GLScissor(m_x, m_y, m_width, m_height);
		}
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
