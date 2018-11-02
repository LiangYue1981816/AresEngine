#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform1i : public CGfxCommandBase
{
public:
	CGfxCommandUniform1i(const char *szName, int v0)
		: m_name(HashValue(szName))
		, m_v0(v0)
	{

	}
	virtual ~CGfxCommandUniform1i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1I, "CommandUniform1i");
		{
			Renderer()->Uniform1i(m_name, m_v0);
		}
	}


private:
	uint32_t m_name;
	int m_v0;
};
