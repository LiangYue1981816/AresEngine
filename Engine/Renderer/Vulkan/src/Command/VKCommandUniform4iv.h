#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform4iv : public CGfxCommandBase
{
public:
	CVKCommandUniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value)
		: m_name(name)
	{
		m_value.assign(value, value + count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform4iv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4IV, "CommandUniform4iv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform4iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform4iv(vkCommandBuffer, m_name, m_value.size(), m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<int> m_value;
};
