#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform4fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform4fv(uint32_t name, int count, const float* value)
		: m_name(name)
		, m_value(value, value + count)
	{

	}
	virtual ~CGLES3CommandUniform4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4FV, "CommandUniform4fv");
		{
			if (CGLES3PipelineCompute* pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform4fv(m_name, m_value.size(), m_value.data());
			}

			if (CGLES3PipelineGraphics* pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform4fv(m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
