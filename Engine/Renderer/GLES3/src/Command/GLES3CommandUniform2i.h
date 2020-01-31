#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2i(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int v0, int v1)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
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
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform2i(m_name, m_v0, m_v1);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform2i(m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
