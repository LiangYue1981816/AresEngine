#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform4f : public CGfxCommandBase
{
public:
	CVKCommandUniform4f(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0, float v1, float v2, float v3)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{
		Execute();
	}
	virtual ~CVKCommandUniform4f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4F, "CommandUniform4f");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform4f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform4f(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
	float m_v3;

	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
