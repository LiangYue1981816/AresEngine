#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix4fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
		, m_value(value, value + count)
	{

	}
	virtual ~CGLES3CommandUniformMatrix4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV, "CommandUniformMatrix4fv");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->UniformMatrix4fv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
