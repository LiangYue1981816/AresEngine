#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix4fv(uint32_t name, int count, const float *value)
		: m_name(name)
		, m_value(value, value + 16 * count)
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
			if (CGLES3PipelineCompute *pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix4fv(m_name, m_value.size() / 16, m_value.data());
			}

			if (CGLES3PipelineGraphics *pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix4fv(m_name, m_value.size() / 16, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
