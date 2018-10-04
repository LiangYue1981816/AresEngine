#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandSetPolygonOffset : public CGfxCommandBase
{
public:
	CGfxCommandSetPolygonOffset(bool bEnable, float factor, float units)
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
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_POLYGONOFFSET, "CommandSetPolygonOffset");
		{
			if (m_bEnable) {
				glEnable(GL_POLYGON_OFFSET_FILL);
			}
			else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}

			glPolygonOffset(m_factor, m_units);
		}
	}


private:
	bool m_bEnable;
	float m_factor;
	float m_units;
};
