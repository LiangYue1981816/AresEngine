#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CVKCommandBindPipelineCompute(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CVKPipelineCompute*)pPipelineCompute)
	{
		Execute();
	}
	virtual ~CVKCommandBindPipelineCompute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineCompute);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE);
		{
			m_pPipelineCompute->Bind(m_vkCommandBuffer);
		}
	}


private:
	CVKPipelineCompute* m_pPipelineCompute;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
