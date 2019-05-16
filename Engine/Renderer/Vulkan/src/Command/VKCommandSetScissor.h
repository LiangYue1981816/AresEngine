#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetScissor : public CGfxCommandBase
{
public:
	CVKCommandSetScissor(VkCommandBuffer vkCommandBuffer, int x, int y, int width, int height)
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandSetScissor(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_SCISSOR, "CommandSetScissor");
		{
			VkRect2D rect = {};
			rect.offset.x = m_x;
			rect.offset.y = m_y;
			rect.extent.width = m_width;
			rect.extent.height = m_height;
			vkCmdSetScissor(vkCommandBuffer, 0, 1, &rect);
		}
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
