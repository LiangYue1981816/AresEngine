#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform3f : public CGfxCommandBase
{
public:
	CGfxCommandUniform3f(const char *szName, float v0, float v1, float v2)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
	{

	}
	virtual ~CGfxCommandUniform3f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3F, "CommandUniform3f");
		{
			Renderer()->Uniform3f(m_name, m_v0, m_v1, m_v2);
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
};
