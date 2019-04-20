#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform3i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform3i(uint32_t name, int v0, int v1, int v2)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
	{

	}
	virtual ~CGLES3CommandUniform3i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3I, "CommandUniform3i");
		{
			if (CGLES3PipelineCompute * pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform3i(m_name, m_v0, m_v1, m_v2);
			}

			if (CGLES3PipelineGraphics * pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform3i(m_name, m_v0, m_v1, m_v2);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;
};
