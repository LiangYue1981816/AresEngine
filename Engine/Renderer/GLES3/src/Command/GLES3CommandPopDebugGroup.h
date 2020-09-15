#pragma once
#include "gl3w.h"
#include "GfxCommandBuffer.h"


class CGLES3CommandPopDebugGroup : public CGfxCommandBase
{
public:
	CGLES3CommandPopDebugGroup(void)
	{

	}
	virtual ~CGLES3CommandPopDebugGroup(void)
	{

	}

public:
	virtual void Execute(void)
	{
#ifdef PLATFORM_WINDOWS
		glPopDebugGroup();
#endif
	}
};
