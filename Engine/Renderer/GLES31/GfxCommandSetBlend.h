#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetBlend : public CGfxCommandBase
{
public:
	CGfxCommandSetBlend(bool bEnable, uint32_t srcFactor, uint32_t dstFactor)
		: m_bEnable(bEnable)
		, m_srcFactor(srcFactor)
		, m_dstFactor(dstFactor)
	{

	}
	virtual ~CGfxCommandSetBlend(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_BLEND, "CommandSetBlend");
		{
			if (m_bEnable) {
				glEnable(GL_BLEND);
			}
			else {
				glDisable(GL_BLEND);
			}

			glBlendFunc(m_srcFactor, m_dstFactor);
		}
	}


private:
	bool m_bEnable;
	uint32_t m_srcFactor;
	uint32_t m_dstFactor;
};
