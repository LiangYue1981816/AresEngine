#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawIndirect : public CGfxCommandBase
{
public:
	CGLES3CommandDrawIndirect(GfxIndexType type, uint32_t offset)
		: m_type(type)
		, m_offset(offset)
	{

	}
	virtual ~CGLES3CommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
#if GLES_VER == 310
			glDrawElementsIndirect(CGLES3Helper::TranslatePrimitiveTopology(GLES3Renderer()->GetCurrentPipelineGraphics()->GetPipelineState().topology), CGLES3Helper::TranslateIndexType(m_type), (const void *)m_offset);
#endif
		}
	}


private:
	GfxIndexType m_type;
	uintptr_t m_offset;
};
