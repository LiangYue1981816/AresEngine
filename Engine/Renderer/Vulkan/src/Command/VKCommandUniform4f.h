#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform4f : public CGfxCommandBase
{
public:
	CVKCommandUniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3)
		: m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandUniform4f(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4F, "CommandUniform4f");
		{
			if (CVKPipelineCompute* pPipeline = VKRenderer()->GetCurrentPipelineCompute()) {
				pPipeline->Uniform4f(vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}

			if (CVKPipelineGraphics* pPipeline = VKRenderer()->GetCurrentPipelineGraphics()) {
				pPipeline->Uniform4f(vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
	float m_v3;
};
