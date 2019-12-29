#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetBufferBarrier : public CGfxCommandBase
{
public:
	CGLES3CommandSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxPipelineStageFlagBits pipelineStage)
		: m_ptrBuffer(ptrBuffer)
		, m_pipelineStage(pipelineStage)
	{

	}
	virtual ~CGLES3CommandSetBufferBarrier(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_BUFFERBARRIER, "CommandSetBufferBarrier");
		{
			if (m_ptrBuffer) {

			}
		}
	}


private:
	CGfxStorageBufferPtr m_ptrBuffer;
	GfxPipelineStageFlagBits m_pipelineStage;
};
