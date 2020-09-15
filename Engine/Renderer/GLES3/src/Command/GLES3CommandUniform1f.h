#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1f(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_v0(v0)
	{

	}
	virtual ~CGLES3CommandUniform1f(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F);
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform1f(m_name, m_v0);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform1f(m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
