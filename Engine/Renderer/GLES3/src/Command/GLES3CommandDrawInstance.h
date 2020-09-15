#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(int indexType, int indexOffset, int indexCount, int instanceCount)
		: m_indexType(indexType)
		, m_indexOffset(indexOffset)
		, m_indexCount(indexCount)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void)
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE);
		{
			glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, CGLES3Helper::TranslateIndexType((GfxIndexType)m_indexType), (const void*)m_indexOffset, m_instanceCount);
		}
	}


private:
	int m_indexType;
	int m_indexOffset;
	int m_indexCount;
	int m_instanceCount;
};
