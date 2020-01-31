#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandDrawIndirect : public CGfxCommandBase
{
public:
	CVKCommandDrawIndirect(VkCommandBuffer vkCommandBuffer, const CGfxIndirectBufferPtr ptrIndirectBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndirectBuffer(ptrIndirectBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrIndirectBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{

		}
	}


private:
	CGfxIndirectBufferPtr m_ptrIndirectBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
