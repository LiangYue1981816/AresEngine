#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform4f : public CGfxCommandBase
{
public:
	CGfxCommandUniform4f(const char *szName, float v0, float v1, float v2, float v3)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{

	}
	virtual ~CGfxCommandUniform4f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4F, "CommandUniform4f");
		{
			Renderer()->Uniform4f(m_name, m_v0, m_v1, m_v2, m_v3);
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
	float m_v3;
};
