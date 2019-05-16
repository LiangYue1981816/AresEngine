#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform3f : public CGfxCommandBase
{
public:
	CVKCommandUniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
	{
		Execute();
	}
	virtual ~CVKCommandUniform3f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3F, "CommandUniform3f");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform3f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform3f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
};
