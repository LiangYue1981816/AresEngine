#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1iv : public CGfxCommandBase
{
public:
	CVKCommandUniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value)
		: m_name(name)
	{
		m_value.assign(value, value + count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform1iv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1IV, "CommandUniform1iv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
