#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandExecute : public CGfxCommandBase
{
public:
	CGLES3CommandExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
		: m_ptrCommandBuffer(ptrCommandBuffer)
	{

	}
	virtual ~CGLES3CommandExecute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_EXECUTE);
		{
			m_ptrCommandBuffer->Execute();
		}
	}


private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;
};
