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
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_DEPTH, "CommandClearDepth");
		{

		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	float m_depth;
};
