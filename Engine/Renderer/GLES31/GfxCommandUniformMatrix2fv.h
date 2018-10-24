#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniformMatrix2fv : public CGfxCommandBase
{
public:
	CGfxCommandUniformMatrix2fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.resize(count);
		memcpy(m_value.data(), value, sizeof(float) * count);
	}
	virtual ~CGfxCommandUniformMatrix2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV, "CommandUniformMatrix2fv");
		{
			Renderer()->UniformMatrix2fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
