#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform3iv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform3iv(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const int* value)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_value(value, value + count)
	{

	}
	virtual ~CGLES3CommandUniform3iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3IV);
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform3iv(m_name, m_value.size(), m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform3iv(m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
