#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(GfxIndexType type, uint32_t offset, int count, int instanceCount)
		: m_type(type)
		, m_offset(offset)
		, m_count(count)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			glDrawElementsInstanced(CGLES3Helper::TranslatePrimitiveTopology(GLES3Renderer()->GetCurrentPipelineGraphics()->GetPipelineState().topology), m_count, CGLES3Helper::TranslateIndexType(m_type), (const void *)m_offset, m_instanceCount);
		}
	}


private:
	GfxIndexType m_type;
	uintptr_t m_offset;
	int m_count;
	int m_instanceCount;
};
