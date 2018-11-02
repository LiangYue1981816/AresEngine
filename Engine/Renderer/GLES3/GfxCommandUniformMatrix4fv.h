#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CGfxCommandUniformMatrix4fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGfxCommandUniformMatrix4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV, "CommandUniformMatrix4fv");
		{
			Renderer()->UniformMatrix4fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
