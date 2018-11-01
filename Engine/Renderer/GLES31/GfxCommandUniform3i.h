#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform3i : public CGfxCommandBase
{
public:
	CGfxCommandUniform3i(const char *szName, int v0, int v1, int v2)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
	{

	}
	virtual ~CGfxCommandUniform3i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3I, "CommandUniform3i");
		{
			Renderer()->Uniform3i(m_name, m_v0, m_v1, m_v2);
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;
};
