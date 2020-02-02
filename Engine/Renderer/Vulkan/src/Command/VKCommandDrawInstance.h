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

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE);
		{
			int indexSize = 4;

			switch (m_indexType) {
			case GFX_INDEX_UNSIGNED_SHORT:
				indexSize = 2;
				break;

			case GFX_INDEX_UNSIGNED_INT:
				indexSize = 4;
				break;
			}

			vkCmdDrawIndexed(m_vkCommandBuffer, m_indexCount, m_instanceCount, m_indexOffset / indexSize, 0, 0);
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
