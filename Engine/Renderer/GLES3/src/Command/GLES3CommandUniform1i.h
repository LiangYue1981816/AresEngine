#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1i(uint32_t name, int v0)
		: m_name(name)
		, m_v0(v0)
	{

	}
	virtual ~CGLES3CommandUniform1i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1I, "CommandUniform1i");
		{
			if (CGLES3PipelineCompute* pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1i(m_name, m_v0);
			}

			if (CGLES3PipelineGraphics* pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1i(m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
};
