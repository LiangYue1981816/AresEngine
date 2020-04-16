#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetViewport : public CGfxCommandBase
{
public:
	CVKCommandSetViewport(VkCommandBuffer vkCommandBuffer, int x, int y, int width, int height, float znear, float zfar)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
		, m_znear(znear)
		, m_zfar(zfar)
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
			viewport.minDepth = m_znear;
			viewport.maxDepth = m_zfar;
			vkCmdSetViewport(m_vkCommandBuffer, 0, 1, &viewport);
		}
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	float m_znear;
	float m_zfar;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
