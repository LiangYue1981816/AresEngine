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

		VkCommandBuffer vkExecuteCommandBuffer = ((CVKCommandBuffer*)m_ptrCommandBuffer.GetPointer())->GetCommandBuffer();
		vkCmdExecuteCommands(m_vkCommandBuffer, 1, &vkExecuteCommandBuffer);
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;
};
