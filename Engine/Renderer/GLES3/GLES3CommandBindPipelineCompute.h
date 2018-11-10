#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CGLES3CommandBindPipelineCompute(CGfxPipelineCompute *pPipelineCompute)
		: m_pPipelineCompute(pPipelineCompute)
	{

	}
	virtual ~CGLES3CommandBindPipelineCompute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE, "CommandBindPipelineCompute");
		{
			if (m_pPipelineCompute) {
				GLES3Renderer()->BindPipelineCompute(m_pPipelineCompute);
			}
			else {
				GLBindProgramPipeline(0);
			}
		}
	}


private:
	CGfxPipelineCompute *m_pPipelineCompute;
};
