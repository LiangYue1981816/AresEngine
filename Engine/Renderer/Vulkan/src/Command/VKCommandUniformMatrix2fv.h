#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix2fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value)
		: m_vkCommandBuffer(vkCommandBuffer)
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
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV, "CommandUniformMatrix2fv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix2fv(m_vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix2fv(m_vkCommandBuffer, m_name, m_value.size() / 4, m_value.data());
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
