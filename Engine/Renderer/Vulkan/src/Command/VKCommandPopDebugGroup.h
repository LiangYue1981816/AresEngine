#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CVKCommandPopDebugGroup : public CGfxCommandBase
{
public:
	CVKCommandPopDebugGroup(VkCommandBuffer vkCommandBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandPopDebugGroup(void)
	{

	}

public:
	virtual void Execute(void) const
	{

	}


private:
	VkCommandBuffer m_vkCommandBuffer;
};
