#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CGLES3CommandPushDebugGroup : public CGfxCommandBase
{
public:
	CGLES3CommandPushDebugGroup(const char* szMessage)
		: m_strMessage(szMessage)
	{

	}
	virtual ~CGLES3CommandPushDebugGroup(void)
	{

	}

public:
	virtual void Execute(void)
	{
#ifdef PLATFORM_WINDOWS
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, m_strMessage.c_str());
#endif
	}


private:
	eastl::string m_strMessage;
};
