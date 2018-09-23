#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetViewport : public CGfxCommandBase
{
public:
	CGfxCommandSetViewport(int x, int y, int width, int height)
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{

	}
	virtual ~CGfxCommandSetViewport(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_VIEWPORT, "CommandSetViewport");
		{
			glViewport(m_x, m_y, m_width, m_height);
		}
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
