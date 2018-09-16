#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandSetPolygonOffset : public CGfxCommandBase
{
public:
	CGfxCommandSetPolygonOffset(bool bEnable, GLfloat factor, GLfloat units)
		: m_bEnable(bEnable)
		, m_factor(factor)
		, m_units(units)
	{

	}
	virtual ~CGfxCommandSetPolygonOffset(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		if (m_bEnable) {
			glEnable(GL_POLYGON_OFFSET_FILL);
		}
		else {
			glDisable(GL_POLYGON_OFFSET_FILL);
		}

		glPolygonOffset(m_factor, m_units);
	}


private:
	bool m_bEnable;
	GLfloat m_factor;
	GLfloat m_units;
};
