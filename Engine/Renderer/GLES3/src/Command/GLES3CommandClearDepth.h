#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandClearDepth : public CGfxCommandBase
{
public:
	CGLES3CommandClearDepth(float depth)
		: m_depth(depth)
	{

	}
	virtual ~CGLES3CommandClearDepth(void)
	{

	}

public:
	virtual void Execute(void)
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_CLEAR_DEPTH);
		{
			GLClearDepthf(m_depth);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}


private:
	float m_depth;
};
