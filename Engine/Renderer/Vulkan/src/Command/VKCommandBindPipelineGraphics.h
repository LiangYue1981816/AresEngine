#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindPipelineGraphics : public CGfxCommandBase
{
public:
	CVKCommandBindPipelineGraphics(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CGfxPipelineGraphics*)pPipelineGraphics)
	{
		Execute();
	}
	virtual ~CVKCommandBindPipelineGraphics(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS, "CommandBindPipelineGraphics");
		{
			VKRenderer()->BindPipelineGraphics(m_vkCommandBuffer, m_pPipelineGraphics);
		}
	}


private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CGfxPipelineGraphics* m_pPipelineGraphics;
};
