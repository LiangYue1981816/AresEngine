#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandExecute : public CGfxCommandBase
{
public:
	CVKCommandExecute(VkCommandBuffer vkCommandBuffer, const CGfxCommandBufferPtr ptrCommandBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrCommandBuffer(ptrCommandBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandExecute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_EXECUTE, "CommandExecute");
		{
			VkCommandBuffer vkExecuteCommandBuffer = ((CVKCommandBuffer*)m_ptrCommandBuffer.GetPointer())->GetCommandBuffer();
			vkCmdExecuteCommands(m_vkCommandBuffer, 1, &vkExecuteCommandBuffer);
		}
	}


private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
