#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2i(uint32_t name, int v0, int v1)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{

	}
	virtual ~CGLES3CommandUniform2i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			if (CGLES3PipelineCompute * pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform2i(m_name, m_v0, m_v1);
			}

			if (CGLES3PipelineGraphics * pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform2i(m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
};
