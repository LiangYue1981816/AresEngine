#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetDepthTest : public CGfxCommandBase
{
public:
	CGfxCommandSetDepthTest(bool bEnable, GLenum depthFunc)
		: m_bEnable(bEnable)
		, m_depthFunc(depthFunc)
	{

	}
	virtual ~CGfxCommandSetDepthTest(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_DEPTHTEST, "CommandSetDepthTest");
		{
			if (m_bEnable) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}

			glDepthFunc(m_depthFunc);
		}
	}


private:
	bool m_bEnable;
	GLenum m_depthFunc;
};
