#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix2fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix2fv(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const float* value)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_name(name)
	{
		m_value.assign(value, value + 4 * count);
		Execute();
	}
	virtual ~CVKCommandUniformMatrix2fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV, "CommandUniformMatrix2fv");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->UniformMatrix2fv(m_vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->UniformMatrix2fv(m_vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;

private:
	VkCommandBuffer m_vkCommandBuffer;
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;
};
