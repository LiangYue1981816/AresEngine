#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetDepthWrite : public CGfxCommandBase
{
public:
	CGfxCommandSetDepthWrite(bool bEnable)
		: m_bEnable(bEnable)
	{

	}
	virtual ~CGfxCommandSetDepthWrite(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_DEPTHWRITE, "CommandSetDepthWrite");
		{
			if (m_bEnable) {
				glDepthMask(GL_TRUE);
			}
			else {
				glDepthMask(GL_FALSE);
			}
		}
	}


private:
	bool m_bEnable;
};
