#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandSetBlend : public CGfxCommandBase
{
public:
	CGfxCommandSetBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor)
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
		if (m_bEnable) {
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}

		glBlendFunc(m_srcFactor, m_dstFactor);
	}


private:
	bool m_bEnable;
	GLenum m_srcFactor;
	GLenum m_dstFactor;
};
