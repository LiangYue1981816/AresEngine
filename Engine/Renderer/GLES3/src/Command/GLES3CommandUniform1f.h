#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1f(uint32_t name, float v0)
		: m_name(name)
		, m_v0(v0)
	{

	}
	virtual ~CGLES3CommandUniform1f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F, "CommandUniform1f");
		{
			if (CGLES3PipelineCompute* pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1f(m_name, m_v0);
			}

			if (CGLES3PipelineGraphics* pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1f(m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
};
