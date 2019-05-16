#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CVKCommandBindPipelineCompute(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineCompute((CGfxPipelineCompute*)pPipelineCompute)
	{
		Execute();
	}
	virtual ~CVKCommandBindPipelineCompute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE, "CommandBindPipelineCompute");
		{
			VKRenderer()->BindPipelineCompute(m_vkCommandBuffer, m_pPipelineCompute);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxPipelineCompute* m_pPipelineCompute;
};
