#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform3fv : public CGfxCommandBase
{
public:
	CGfxCommandUniform3fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.resize(count);
		memcpy(m_value.data(), value, sizeof(float) * count);
	}
	virtual ~CGfxCommandUniform3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3FV, "CommandUniform3fv");
		{
			Renderer()->Uniform3fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
