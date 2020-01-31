#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform3f : public CGfxCommandBase
{
public:
	CVKCommandUniform3f(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0, float v1, float v2)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
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
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM3F, "CommandUniform3f");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform3f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform3f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;

private:
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
