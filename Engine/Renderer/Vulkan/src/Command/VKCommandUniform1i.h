#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1i : public CGfxCommandBase
{
public:
	CVKCommandUniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0)
		: m_name(name)
		, m_v0(v0)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform1i(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1I, "CommandUniform1i");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1i(vkCommandBuffer, m_name, m_v0);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1i(vkCommandBuffer, m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
};
