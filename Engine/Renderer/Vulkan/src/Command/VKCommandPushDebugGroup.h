#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CVKCommandPushDebugGroup : public CGfxCommandBase
{
public:
	CVKCommandPushDebugGroup(VkCommandBuffer vkCommandBuffer, const char* szMessage)
		: m_strMessage(szMessage)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandPushDebugGroup(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{

	}


private:
	eastl::string m_strMessage;
};
