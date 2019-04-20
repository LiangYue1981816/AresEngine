#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix3fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix3fv(uint32_t name, int count, const float* value)
		: m_name(name)
	{
		m_value.assign(value, value + 9 * count);
	}
	virtual ~CGLES3CommandUniformMatrix3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV, "CommandUniformMatrix3fv");
		{
			if (CGLES3PipelineCompute * pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix3fv(m_name, m_value.size() / 9, m_value.data());
			}

			if (CGLES3PipelineGraphics * pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix3fv(m_name, m_value.size() / 9, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
