#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawIndirect : public CGfxCommandBase
{
public:
	CGLES3CommandDrawIndirect(GfxDrawMode mode, GfxIndexType type, uint32_t offset)
		: m_mode(mode)
		, m_type(type)
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
			glDrawElementsIndirect(GLDrawMode(m_mode), GLIndexType(m_type), (const void *)m_offset);
#endif
		}
	}


private:
	GfxDrawMode m_mode;
	GfxIndexType m_type;
	uintptr_t m_offset;
};
