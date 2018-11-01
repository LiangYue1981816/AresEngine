#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform2iv : public CGfxCommandBase
{
public:
	CGfxCommandUniform2iv(const char *szName, int count, const int *value)
		: m_name(HashValue(szName))
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGfxCommandUniform2iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2IV, "CommandUniform2iv");
		{
			Renderer()->Uniform2iv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
