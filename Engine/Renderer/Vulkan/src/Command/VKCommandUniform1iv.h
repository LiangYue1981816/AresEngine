#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1iv : public CGfxCommandBase
{
public:
	CVKCommandUniform1iv(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int count, const int* value)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_name(name)
	{
		m_value.assign(value, value + count);
		Execute();
	}
	virtual ~CVKCommandUniform1iv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1IV, "CommandUniform1iv");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform1iv(m_vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform1iv(m_vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;

private:
	VkCommandBuffer m_vkCommandBuffer;
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;
};
