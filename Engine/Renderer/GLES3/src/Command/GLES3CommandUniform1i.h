#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1i(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int v0)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
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
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform1i(m_name, m_v0);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform1i(m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
