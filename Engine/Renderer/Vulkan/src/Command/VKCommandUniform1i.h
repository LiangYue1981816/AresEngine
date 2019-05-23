#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1i : public CGfxCommandBase
{
public:
	CVKCommandUniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
		, m_v0(v0)
	{
		Execute();
	}
	virtual ~CVKCommandUniform1i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1I, "CommandUniform1i");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform1i(m_vkCommandBuffer, m_name, m_v0);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform1i(m_vkCommandBuffer, m_name, m_v0);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	int m_v0;
};
