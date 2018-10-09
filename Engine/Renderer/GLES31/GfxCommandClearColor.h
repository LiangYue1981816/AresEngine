#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandClearColor : public CGfxCommandBase
{
public:
	CGfxCommandClearColor(float red, float green, float blue, float alpha)
		: m_red(red)
		, m_green(green)
		, m_blue(blue)
		, m_alpha(alpha)
	{

	}
	virtual ~CGfxCommandClearColor(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_COLOR, "CommandClearColor");
		{
			GLClearColor(m_red, m_green, m_blue, m_alpha);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}


private:
	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;
};
