#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2f : public CGfxCommandBase
{
public:
	CVKCommandUniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform2f(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2F, "CommandUniform2f");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform2f(vkCommandBuffer, m_name, m_v0, m_v1);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform2f(vkCommandBuffer, m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
};
