#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix4fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value)
		: m_name(name)
	{
		m_value.assign(value, value + 16 * count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniformMatrix4fv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV, "CommandUniformMatrix4fv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix4fv(vkCommandBuffer, m_name, m_value.size() / 16, m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix4fv(vkCommandBuffer, m_name, m_value.size() / 16, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
