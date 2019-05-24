#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix4fv(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const float* value)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_name(name)
	{
		m_value.assign(value, value + 16 * count);
		Execute();
	}
	virtual ~CVKCommandUniformMatrix4fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV, "CommandUniformMatrix4fv");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->UniformMatrix4fv(m_vkCommandBuffer, m_name, m_value.size() / 16, m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->UniformMatrix4fv(m_vkCommandBuffer, m_name, m_value.size() / 16, m_value.data());
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
