#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CVKCommandPushDebugGroup : public CGfxCommandBase
{
public:
	CVKCommandPushDebugGroup(VkCommandBuffer vkCommandBuffer, const char* szMessage)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_strMessage(szMessage)
	{
		Execute();
	}
	virtual ~CVKCommandPushDebugGroup(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	eastl::string m_strMessage;
};
