#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandSetScissor : public CGfxCommandBase
{
public:
	CGfxCommandSetScissor(int x, int y, int width, int height)
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{

	}
	virtual ~CGfxCommandSetScissor(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_x, m_y, m_width, m_height);
	}


private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
