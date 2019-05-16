#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandClearDepth : public CGfxCommandBase
{
public:
	CVKCommandClearDepth(VkCommandBuffer vkCommandBuffer, float depth)
		: m_depth(depth)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandClearDepth(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_DEPTH, "CommandClearDepth");
		{

		}
	}


private:
	float m_depth;
};
