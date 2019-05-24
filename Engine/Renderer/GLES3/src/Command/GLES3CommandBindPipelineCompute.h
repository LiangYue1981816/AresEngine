#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindPipelineCompute : public CGfxCommandBase
{
public:
	CGLES3CommandBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
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
			m_pPipelineCompute->Bind();
		}
	}


private:
	CGLES3PipelineCompute* m_pPipelineCompute;
};
