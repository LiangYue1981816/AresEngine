#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawIndirect : public CGfxCommandBase
{
public:
	CGfxCommandDrawIndirect(uint32_t mode, uint32_t type, int count, int baseIndex, int baseVertex, int instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_count(count)
		, m_baseIndex(baseIndex)
		, m_baseVertex(baseVertex)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGfxCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			struct {
				uint32_t count;
				uint32_t instanceCount;
				uint32_t baseIndex;
				uint32_t baseVertex;
				uint32_t reserved;
			} indirect;

			indirect.count = m_count;
			indirect.baseIndex = m_baseIndex;
			indirect.baseVertex = m_baseVertex;
			indirect.instanceCount = m_instanceCount;
			indirect.reserved = 0;

			glDrawElementsIndirect(m_mode, m_type, (const void *)&indirect);
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	int m_count;
	int m_baseIndex;
	int m_baseVertex;
	int m_instanceCount;
};
