#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandEndRecord : public CGfxCommandBase
{
public:
	CVKCommandEndRecord(VkCommandBuffer vkCommandBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandEndRecord(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_END_RECORD);
		{
			vkEndCommandBuffer(m_vkCommandBuffer);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;
};
