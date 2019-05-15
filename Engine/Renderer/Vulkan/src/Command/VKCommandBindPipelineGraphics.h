#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindPipelineGraphics : public CGfxCommandBase
{
public:
	CVKCommandBindPipelineGraphics(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
		: m_pPipelineGraphics((CGfxPipelineGraphics*)pPipelineGraphics)
	{
		Execute(vkCommandBuffer);
	}
	virtual ~CVKCommandBindPipelineGraphics(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS, "CommandBindPipelineGraphics");
		{
			VKRenderer()->BindPipelineGraphics(vkCommandBuffer, m_pPipelineGraphics);
		}
	}


private:
	CGfxPipelineGraphics* m_pPipelineGraphics;
};
