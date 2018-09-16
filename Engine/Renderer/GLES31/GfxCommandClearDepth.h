#pragma once
#include <gl31.h>
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
		glClearDepthf(m_depth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}


private:
	float m_depth;
};
