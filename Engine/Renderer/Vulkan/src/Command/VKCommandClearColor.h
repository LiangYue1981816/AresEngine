#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandClearColor : public CGfxCommandBase
{
public:
	CVKCommandClearColor(VkCommandBuffer vkCommandBuffer, float red, float green, float blue, float alpha)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_red(red)
		, m_green(green)
		, m_blue(blue)
		, m_alpha(alpha)
	{
		Execute();
	}
	virtual ~CVKCommandClearColor(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_COLOR, "CommandClearColor");
		{

		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;
};
