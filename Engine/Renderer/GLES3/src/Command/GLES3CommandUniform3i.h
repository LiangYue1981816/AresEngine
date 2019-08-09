#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform3i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform3i(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int v0, int v1, int v2)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
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
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3I, "CommandUniform3i");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform3i(m_name, m_v0, m_v1, m_v2);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform3i(m_name, m_v0, m_v1, m_v2);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;

	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
