#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix3fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix3fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
		, m_value(value, value + 9 * count)
	{

	}
	virtual ~CGLES3CommandUniformMatrix3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV, "CommandUniformMatrix3fv");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->UniformMatrix3fv(m_name, m_value.size() / 9, m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
