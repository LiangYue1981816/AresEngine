#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform4f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{

	}
	virtual ~CGLES3CommandUniform4f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4F, "CommandUniform4f");
		{
			if (CGLES3PipelineCompute * pPipeline = GLES3Renderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform4f(m_name, m_v0, m_v1, m_v2, m_v3);
			}

			if (CGLES3PipelineGraphics * pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform4f(m_name, m_v0, m_v1, m_v2, m_v3);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
	float m_v3;
};
