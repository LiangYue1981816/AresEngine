#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform1f : public CGfxCommandBase
{
public:
	CGfxCommandUniform1f(const char *szName, float v0)
		: m_name(HashValue(szName))
		, m_v0(v0)
	{

	}
	virtual ~CGfxCommandUniform1f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F, "CommandUniform1f");
		{
			Renderer()->Uniform1f(m_name, m_v0);
		}
	}


private:
	uint32_t m_name;
	float m_v0;
};
