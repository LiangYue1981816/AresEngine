#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1f : public CGfxCommandBase
{
public:
	CVKCommandUniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
		, m_v0(v0)
	{
		Execute();
	}
	virtual ~CVKCommandUniform1f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F, "CommandUniform1f");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1f(m_vkCommandBuffer, m_name, m_v0);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1f(m_vkCommandBuffer, m_name, m_v0);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	float m_v0;
};
