#pragma once
#include <gl31.h>
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
		if (m_bEnable) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}

		glDepthFunc(m_depthFunc);
	}


private:
	bool m_bEnable;
	GLenum m_depthFunc;
};
