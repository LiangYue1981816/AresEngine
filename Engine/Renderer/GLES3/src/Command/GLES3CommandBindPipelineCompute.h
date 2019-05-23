#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CGLES3CommandBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
		: m_pPipelineCompute((CGfxPipelineCompute*)pPipelineCompute)
	{

	}
	virtual ~CGLES3CommandBindPipelineCompute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_pPipelineCompute);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE, "CommandBindPipelineCompute");
		{
			GLES3Renderer()->BindPipelineCompute(m_pPipelineCompute);
		}
	}


private:
	CGfxPipelineCompute* m_pPipelineCompute;
};
