#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1f : public CGfxCommandBase
{
public:
	CVKCommandUniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0)
		: m_name(name)
		, m_v0(v0)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform1f(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F, "CommandUniform1f");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1f(vkCommandBuffer, m_name, m_v0);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1f(vkCommandBuffer, m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
};
