#pragma once
#include <gl31.h>
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
		if (m_bEnable) {
			glDepthMask(GL_TRUE);
		}
		else {
			glDepthMask(GL_FALSE);
		}
	}


private:
	bool m_bEnable;
};
