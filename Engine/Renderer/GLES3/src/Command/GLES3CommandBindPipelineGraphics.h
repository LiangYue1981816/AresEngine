#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindPipelineGraphics : public CGfxCommandBase
{
public:
	CGLES3CommandBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
		: m_pPipelineGraphics((CGfxPipelineGraphics*)pPipelineGraphics)
	{

	}
	virtual ~CGLES3CommandBindPipelineGraphics(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS, "CommandBindPipelineGraphics");
		{
			GLES3Renderer()->BindPipelineGraphics(m_pPipelineGraphics);
		}
	}


private:
	CGfxPipelineGraphics* m_pPipelineGraphics;
};
