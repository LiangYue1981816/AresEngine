#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandClearColor : public CGfxCommandBase
{
public:
	CGLES3CommandClearColor(float red, float green, float blue, float alpha)
		: m_red(red)
		, m_green(green)
		, m_blue(blue)
		, m_alpha(alpha)
	{

	}
	virtual ~CGLES3CommandClearColor(void)
	{

	}

public:
	virtual void Execute(void)
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_COLOR);
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
