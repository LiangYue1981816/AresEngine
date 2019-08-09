#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2fv(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const float* value)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_value(value, value + count)
	{

	}
	virtual ~CGLES3CommandUniform2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2FV, "CommandUniform2fv");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform2fv(m_name, m_value.size(), m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform2fv(m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;

	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
