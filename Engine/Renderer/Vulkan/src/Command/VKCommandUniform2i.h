#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUniform2i : public CGfxCommandBase
{
public:
	CVKCommandUniform2i(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, int v0, int v1)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
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
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2I, "CommandUniform2i");
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform2i(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform2i(m_vkCommandBuffer, m_name, m_v0, m_v1);
			}
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;

private:
	VkCommandBuffer m_vkCommandBuffer;
	CVKPipelineCompute* m_pPipelineCompute;
	CVKPipelineGraphics* m_pPipelineGraphics;
};
