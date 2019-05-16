#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandEndRecord : public CGfxCommandBase
{
public:
	CVKCommandEndRecord(VkCommandBuffer vkCommandBuffer)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandEndRecord(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RECORD, "CommandEndRecord");
		{

		}
	}
};
