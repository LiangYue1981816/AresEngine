#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandSetCullFace : public CGfxCommandBase
{
public:
	CGfxCommandSetCullFace(bool bEnable, GLenum cullFace, GLenum frontFace)
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
		if (m_bEnable) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}

		glCullFace(m_cullFace);
		glFrontFace(m_frontFace);
	}


private:
	bool m_bEnable;
	GLenum m_cullFace;
	GLenum m_frontFace;
};
