#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform4fv : public CGfxCommandBase
{
public:
	CGfxCommandUniform4fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.resize(count);
		memcpy(m_value.data(), value, sizeof(float) * count);
	}
	virtual ~CGfxCommandUniform4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4FV, "CommandUniform4fv");
		{
			Renderer()->Uniform4fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
