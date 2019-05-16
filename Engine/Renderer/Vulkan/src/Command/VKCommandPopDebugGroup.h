#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CVKCommandPopDebugGroup : public CGfxCommandBase
{
public:
	CVKCommandPopDebugGroup(VkCommandBuffer vkCommandBuffer)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandPopDebugGroup(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{

	}
};
