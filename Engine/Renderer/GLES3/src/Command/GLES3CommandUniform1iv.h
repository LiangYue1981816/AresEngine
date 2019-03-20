#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1iv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1iv(const char *szName, int count, const int *value)
		: m_name(HashValue(szName))
		, m_value(value, value + count)
	{

	}
	virtual ~CGLES3CommandUniform1iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1IV, "CommandUniform1iv");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->Uniform1iv(m_name, m_value.size(), m_value.data());
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
