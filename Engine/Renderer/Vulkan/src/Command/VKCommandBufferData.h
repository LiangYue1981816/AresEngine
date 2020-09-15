#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBufferData : public CGfxCommandBase
{
public:
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandBufferData(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
	}


private:

private:
	VkCommandBuffer m_vkCommandBuffer;
};
