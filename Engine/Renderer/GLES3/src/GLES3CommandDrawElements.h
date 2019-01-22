#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawElements : public CGfxCommandBase
{
public:
	CGLES3CommandDrawElements(GfxIndexType type, uint32_t offset, int count)
		: m_type(type)
		, m_offset(offset)
		, m_count(count)
	{

	}
	virtual ~CGLES3CommandDrawElements(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_ELEMENTS, "CommandDrawElements");
		{
			glDrawElements(CGLES3Helper::TranslatePrimitiveTopology(GLES3Renderer()->GetCurrentPipelineGraphics()->GetPipelineState().topology), m_count, CGLES3Helper::TranslateIndexType(m_type), (const void *)m_offset);
		}
	}


private:
	GfxIndexType m_type;
	uintptr_t m_offset;
	int m_count;
};
