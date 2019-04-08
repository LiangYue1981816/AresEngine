#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix2fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix2fv(const char *szName, int count, const float *value)
		: m_name(HashValue(szName))
		, m_value(value, value + 4 * count)
	{

	}
	virtual ~CGLES3CommandUniformMatrix2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV, "CommandUniformMatrix2fv");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->UniformMatrix2fv(m_name, m_value.size() / 4, m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
