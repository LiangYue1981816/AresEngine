#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandSetColorWrite : public CGfxCommandBase
{
public:
	CGfxCommandSetColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha)
		: m_bEnableRed(bEnableRed)
		, m_bEnableGreen(bEnableGreen)
		, m_bEnableBlue(bEnableBlue)
		, m_bEnableAlpha(bEnableAlpha)
	{

	}
	virtual ~CGfxCommandSetColorWrite(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		glColorMask(m_bEnableRed ? GL_TRUE : GL_FALSE, m_bEnableGreen ? GL_TRUE : GL_FALSE, m_bEnableBlue ? GL_TRUE : GL_FALSE, m_bEnableAlpha ? GL_TRUE : GL_FALSE);
	}


private:
	bool m_bEnableRed;
	bool m_bEnableGreen;
	bool m_bEnableBlue;
	bool m_bEnableAlpha;
};
