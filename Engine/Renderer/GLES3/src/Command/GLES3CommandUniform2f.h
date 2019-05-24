#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2f(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0, float v1)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{

	}
	virtual ~CGLES3CommandUniform2f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2F, "CommandUniform2f");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform2f(m_name, m_v0, m_v1);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform2f(m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
