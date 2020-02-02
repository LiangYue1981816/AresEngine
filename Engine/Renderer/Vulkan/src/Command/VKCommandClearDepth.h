#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandClearDepth : public CGfxCommandBase
{
public:
	CVKCommandClearDepth(VkCommandBuffer vkCommandBuffer, float depth)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_depth(depth)
	{
		Execute();
	}
	virtual ~CVKCommandClearDepth(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_DEPTH);
		{

		}
	}


private:
	float m_depth;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
