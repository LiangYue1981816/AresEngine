#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform3iv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform3iv(uint32_t name, int count, const int* value)
		: m_name(name)
	{
		m_value.assign(value, value + count);
	}
	virtual ~CGLES3CommandUniform3iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3IV, "CommandUniform3iv");
		{
			if (CGLES3PipelineCompute* pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform3iv(m_name, m_value.size(), m_value.data());
			}

			if (CGLES3PipelineGraphics* pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform3iv(m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
