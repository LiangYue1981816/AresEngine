#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetViewport : public CGfxCommandBase
{
public:
	CVKCommandSetViewport(VkCommandBuffer vkCommandBuffer, int x, int y, int width, int height)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{
		Execute();
	}
	virtual ~CVKCommandSetViewport(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_VIEWPORT);
		{
			VkViewport viewport = {};
			viewport.x = m_x;
			viewport.y = m_y;
			viewport.width = m_width;
			viewport.height = m_height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_vkCommandBuffer, 0, 1, &viewport);
		}
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
