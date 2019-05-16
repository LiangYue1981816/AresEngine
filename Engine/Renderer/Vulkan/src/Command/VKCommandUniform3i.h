#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform3i : public CGfxCommandBase
{
public:
	CVKCommandUniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
	{
		Execute();
	}
	virtual ~CVKCommandUniform3i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3I, "CommandUniform3i");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform3i(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform3i(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;
};
