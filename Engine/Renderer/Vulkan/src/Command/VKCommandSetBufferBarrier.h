#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetBufferBarrier : public CGfxCommandBase
{
public:
	CVKCommandSetBufferBarrier(VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxPipelineStageFlagBits pipelineStage)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrBuffer(ptrBuffer)
		, m_pipelineStage(pipelineStage)
	{
		Execute();
	}
	virtual ~CVKCommandSetBufferBarrier(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_BUFFERBARRIER, "CommandSetBufferBarrier");
		{
			if (m_ptrBuffer) {
				((CVKStorageBuffer*)m_ptrBuffer.GetPointer())->PipelineBarrier(m_vkCommandBuffer, m_pipelineStage, 0, m_ptrBuffer->GetSize());
			}
		}
	}


private:
	CGfxStorageBufferPtr m_ptrBuffer;
	GfxPipelineStageFlagBits m_pipelineStage;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
