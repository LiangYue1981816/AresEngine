#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform4iv : public CGfxCommandBase
{
public:
	CGfxCommandUniform4iv(const char *szName, int count, const int *value)
		: m_name(HashValue(szName))
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGfxCommandUniform4iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4IV, "CommandUniform4iv");
		{
			Renderer()->Uniform4iv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
