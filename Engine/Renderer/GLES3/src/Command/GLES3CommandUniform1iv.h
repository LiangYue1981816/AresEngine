#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1iv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1iv(uint32_t name, int count, const int *value)
		: m_name(name)
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
			if (CGLES3PipelineCompute *pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1iv(m_name, m_value.size(), m_value.data());
			}

			if (CGLES3PipelineGraphics *pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1iv(m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
