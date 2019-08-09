#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform4i : public CGfxCommandBase
{
public:
	CVKCommandUniform4i(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int v0, int v1, int v2, int v3)
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
	virtual ~CVKCommandUniform4i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4I, "CommandUniform4i");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform4i(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform4i(m_vkCommandBuffer, m_name, m_v0, m_v1, m_v2, m_v3);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;
	int m_v3;

	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
