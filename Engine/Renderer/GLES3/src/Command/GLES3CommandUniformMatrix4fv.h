#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CGLES3CommandUniformMatrix4fv(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const float* value)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
	{
		m_value.assign(value, value + 16 * count);
	}
	virtual ~CGLES3CommandUniformMatrix4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV, "CommandUniformMatrix4fv");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->UniformMatrix4fv(m_name, m_value.size() / 16, m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->UniformMatrix4fv(m_name, m_value.size() / 16, m_value.data());
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
