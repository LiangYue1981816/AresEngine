#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniform2fv : public CGfxCommandBase
{
public:
	CGfxCommandUniform2fv(const char *szName, uint32_t count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.resize(count);
		memcpy(m_value.data(), value, sizeof(float) * count);
	}
	virtual ~CGfxCommandUniform2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2FV, "CommandUniform2fv");
		{
			Renderer()->Uniform2fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
