#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDispatch : public CGfxCommandBase
{
public:
	CGLES3CommandDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
		: m_numLocalWorkGroupX(numLocalWorkGroupX)
		, m_numLocalWorkGroupY(numLocalWorkGroupY)
		, m_numLocalWorkGroupZ(numLocalWorkGroupZ)
	{

	}
	virtual ~CGLES3CommandDispatch(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DISPATCH, "CommandDispatch");
		{
			glDispatchCompute(m_numLocalWorkGroupX, m_numLocalWorkGroupY, m_numLocalWorkGroupZ);
		}
	}


private:
	int m_numLocalWorkGroupX;
	int m_numLocalWorkGroupY;
	int m_numLocalWorkGroupZ;
};
