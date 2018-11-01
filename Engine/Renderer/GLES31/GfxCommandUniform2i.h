#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform2i : public CGfxCommandBase
{
public:
	CGfxCommandUniform2i(const char *szName, int v0, int v1)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
	{

	}
	virtual ~CGfxCommandUniform2i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			Renderer()->Uniform2i(m_name, m_v0, m_v1);
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
};
