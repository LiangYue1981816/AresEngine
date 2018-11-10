#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform3fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform3fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGLES3CommandUniform3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3FV, "CommandUniform3fv");
		{
			GLES3Renderer()->Uniform3fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
