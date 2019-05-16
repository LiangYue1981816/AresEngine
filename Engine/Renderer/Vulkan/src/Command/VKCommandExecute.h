#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandExecute : public CGfxCommandBase
{
public:
	CVKCommandExecute(VkCommandBuffer vkCommandBuffer, const CGfxCommandBufferPtr ptrCommandBuffer)
		: m_ptrCommandBuffer(ptrCommandBuffer)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandExecute(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		if (m_ptrCommandBuffer) {
			VkCommandBuffer vkExecuteCommandBuffer = ((CVKCommandBuffer*)m_ptrCommandBuffer.GetPointer())->GetCommandBuffer();
			vkCmdExecuteCommands(vkCommandBuffer, 1, &vkExecuteCommandBuffer);
		}
	}


private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;
};
