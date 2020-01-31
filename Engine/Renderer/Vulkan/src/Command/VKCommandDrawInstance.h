#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawInstance : public CGfxCommandBase
{
public:
	CVKCommandDrawInstance(VkCommandBuffer vkCommandBuffer, int indexType, int indexOffset, int indexCount, int instanceCount)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_indexType(indexType)
		, m_indexOffset(indexOffset)
		, m_indexCount(indexCount)
		, m_instanceCount(instanceCount)
	{
		Execute();
	}
	virtual ~CVKCommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			vkCmdDrawIndexed(m_vkCommandBuffer, m_indexCount, m_instanceCount, m_indexOffset, 0, 0);
		}
	}


private:
	int m_indexType;
	int m_indexOffset;
	int m_indexCount;
	int m_instanceCount;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
