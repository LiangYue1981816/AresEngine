#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix3fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix3fv(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const float* value)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_value(value, value + 9 * count)
	{

	}
	virtual ~CGLES3CommandUniformMatrix3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV);
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->UniformMatrix3fv(m_name, m_value.size() / 9, m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->UniformMatrix3fv(m_name, m_value.size() / 9, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
