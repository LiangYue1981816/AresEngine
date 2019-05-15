#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CVKCommandBindPipelineCompute(VkCommandBuffer vkCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
		: m_pPipelineCompute((CGfxPipelineCompute*)pPipelineCompute)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandBindPipelineCompute(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE, "CommandBindPipelineCompute");
		{
			VKRenderer()->BindPipelineCompute(vkCommandBuffer, m_pPipelineCompute);
		}
	}


private:
	CGfxPipelineCompute* m_pPipelineCompute;
};
