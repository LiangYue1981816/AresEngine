#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform3fv : public CGfxCommandBase
{
public:
	CVKCommandUniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value)
		: m_name(name)
	{
		m_value.assign(value, value + count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform3fv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3FV, "CommandUniform3fv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform3fv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform3fv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
