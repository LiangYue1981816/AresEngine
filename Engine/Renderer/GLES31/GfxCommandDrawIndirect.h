#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandDrawIndirect : public CGfxCommandBase
{
public:
	CGfxCommandDrawIndirect(uint32_t mode, uint32_t type, uint32_t offset)
		: m_mode(mode)
		, m_type(type)
		, m_offset(offset)

		, m_baseVertex(0)
		, m_firstIndex(0)
		, m_indexCount(0)
		, m_instanceCount(0)
	{

	}
	CGfxCommandDrawIndirect(uint32_t mode, uint32_t type, int baseVertex, int firstIndex, int indexCount, int instanceCount)
		: m_mode(mode)
		, m_type(type)
		, m_offset(INVALID_VALUE)

		, m_baseVertex(baseVertex)
		, m_firstIndex(firstIndex)
		, m_indexCount(indexCount)
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
			if (m_offset != INVALID_VALUE) {
				glDrawElementsIndirect(m_mode, m_type, (const void *)m_offset);
			}
			else {
				typedef struct {
					int indexCount;
					int instanceCount;
					int firstIndex;
					int baseVertex;
					int reservedMustBeZero;
				} DrawCommand;

				DrawCommand drawCommand;
				drawCommand.baseVertex = m_baseVertex;
				drawCommand.firstIndex = m_firstIndex;
				drawCommand.indexCount = m_indexCount;
				drawCommand.instanceCount = m_instanceCount;
				drawCommand.reservedMustBeZero = 0;
				glDrawElementsIndirect(m_mode, m_type, (const void *)&drawCommand);
			}
		}
	}


private:
	uint32_t m_mode;
	uint32_t m_type;
	uint32_t m_offset;

	int m_baseVertex;
	int m_firstIndex;
	int m_indexCount;
	int m_instanceCount;
};
