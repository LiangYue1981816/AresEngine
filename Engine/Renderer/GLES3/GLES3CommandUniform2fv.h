#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGLES3CommandUniform2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2FV, "CommandUniform2fv");
		{
			GLES3Renderer()->Uniform2fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
