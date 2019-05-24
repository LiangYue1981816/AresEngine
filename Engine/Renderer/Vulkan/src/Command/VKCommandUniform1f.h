#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform1f : public CGfxCommandBase
{
public:
	CVKCommandUniform1f(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
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
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1F, "CommandUniform1f");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform1f(m_vkCommandBuffer, m_name, m_v0);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform1f(m_vkCommandBuffer, m_name, m_v0);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;

private:
	VkCommandBuffer m_vkCommandBuffer;
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;
};
