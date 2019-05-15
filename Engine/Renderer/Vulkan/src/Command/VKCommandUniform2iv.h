#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2iv : public CGfxCommandBase
{
public:
	CVKCommandUniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value)
		: m_name(name)
	{
		m_value.assign(value, value + count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform2iv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2IV, "CommandUniform2iv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform2iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform2iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
