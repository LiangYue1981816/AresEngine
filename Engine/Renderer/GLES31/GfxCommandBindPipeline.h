#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindPipeline : public CGfxCommandBase
{
public:
	CGfxCommandBindPipeline(CGfxPipelineBase *pPipeline)
		: m_pPipeline(pPipeline)
	{

	}
	virtual ~CGfxCommandBindPipeline(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINE, "CommandBindPipeline");
		{
			if (m_pPipeline) {
				Renderer()->BindPipeline(m_pPipeline);
			}
		}
	}


private:
	CGfxPipelineBase *m_pPipeline;
};
