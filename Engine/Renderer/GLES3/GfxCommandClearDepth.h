#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandClearDepth : public CGfxCommandBase
{
public:
	CGfxCommandClearDepth(float depth)
		: m_depth(depth)
	{

	}
	virtual ~CGfxCommandClearDepth(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_DEPTH, "CommandClearDepth");
		{
			GLClearDepthf(m_depth);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}


private:
	float m_depth;
};
