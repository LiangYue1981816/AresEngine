#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetCullFace : public CGfxCommandBase
{
public:
	CGfxCommandSetCullFace(bool bEnable, uint32_t cullFace, uint32_t frontFace)
		: m_bEnable(bEnable)
		, m_cullFace(cullFace)
		, m_frontFace(frontFace)
	{

	}
	virtual ~CGfxCommandSetCullFace(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_CULLFACE, "CommandSetCullFace");
		{
			if (m_bEnable) {
				glEnable(GL_CULL_FACE);
			}
			else {
				glDisable(GL_CULL_FACE);
			}

			glCullFace(m_cullFace);
			glFrontFace(m_frontFace);
		}
	}


private:
	bool m_bEnable;
	uint32_t m_cullFace;
	uint32_t m_frontFace;
};
