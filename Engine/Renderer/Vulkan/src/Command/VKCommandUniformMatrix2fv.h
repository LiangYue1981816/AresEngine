#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix2fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value)
		: m_name(name)
	{
		m_value.assign(value, value + 4 * count);
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniformMatrix2fv(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV, "CommandUniformMatrix2fv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix2fv(vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix2fv(vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}
		}
	}


private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
