#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandClearColor : public CGfxCommandBase
{
public:
	CVKCommandClearColor(VkCommandBuffer vkCommandBuffer, float red, float green, float blue, float alpha)
		: m_red(red)
		, m_green(green)
		, m_blue(blue)
		, m_alpha(alpha)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandClearColor(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_COLOR, "CommandClearColor");
		{

		}
	}


private:
	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;
};
