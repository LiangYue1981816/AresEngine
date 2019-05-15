#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2i : public CGfxCommandBase
{
public:
	CVKCommandUniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform2i(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform2i(vkCommandBuffer, m_name, m_v0, m_v1);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform2i(vkCommandBuffer, m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
};
