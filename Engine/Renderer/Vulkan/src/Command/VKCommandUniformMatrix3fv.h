#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniformMatrix3fv : public CGfxCommandBase
{
public:
	CVKCommandUniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
	{
		m_value.assign(value, value + 9 * count);
		Execute();
	}
	virtual ~CVKCommandUniformMatrix3fv(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV, "CommandUniformMatrix3fv");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->UniformMatrix3fv(m_vkCommandBuffer, m_name, m_value.size() / 9, m_value.data());
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->UniformMatrix3fv(m_vkCommandBuffer, m_name, m_value.size() / 9, m_value.data());
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	eastl::vector<float> m_value;
};
