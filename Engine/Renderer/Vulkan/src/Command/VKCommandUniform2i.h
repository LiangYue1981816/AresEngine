#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2i : public CGfxCommandBase
{
public:
	CVKCommandUniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{
		Execute();
	}
	virtual ~CVKCommandUniform2i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform2i(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform2i(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
};
