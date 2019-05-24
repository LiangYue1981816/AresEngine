#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2f : public CGfxCommandBase
{
public:
	CVKCommandUniform2f(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0, float v1)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
	{
		Execute();
	}
	virtual ~CVKCommandUniform2f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2F, "CommandUniform2f");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform2f(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform2f(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;

private:
	VkCommandBuffer m_vkCommandBuffer;
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;
};
