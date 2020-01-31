#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDispatch : public CGfxCommandBase
{
public:
	CVKCommandDispatch(VkCommandBuffer vkCommandBuffer, int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_numLocalWorkGroupX(numLocalWorkGroupX)
		, m_numLocalWorkGroupY(numLocalWorkGroupY)
		, m_numLocalWorkGroupZ(numLocalWorkGroupZ)
	{
		Execute();
	}
	virtual ~CVKCommandDispatch(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DISPATCH, "CommandDispatch");
		{
			vkCmdDispatch(m_vkCommandBuffer, m_numLocalWorkGroupX, m_numLocalWorkGroupY, m_numLocalWorkGroupZ);
		}
	}


private:
	int m_numLocalWorkGroupX;
	int m_numLocalWorkGroupY;
	int m_numLocalWorkGroupZ;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
