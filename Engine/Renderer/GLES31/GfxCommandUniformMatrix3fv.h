#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniformMatrix3fv : public CGfxCommandBase
{
public:
	CGfxCommandUniformMatrix3fv(const char *szName, uint32_t count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.resize(count);
		memcpy(m_value.data(), value, sizeof(float) * count);
	}
	virtual ~CGfxCommandUniformMatrix3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV, "CommandUniformMatrix3fv");
		{
			Renderer()->UniformMatrix3fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
