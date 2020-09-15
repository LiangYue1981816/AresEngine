#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetBufferBarrier : public CGfxCommandBase
{
public:
	CVKCommandSetBufferBarrier(VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrBuffer(ptrBuffer)
		, m_srcAccessFlags(srcAccessFlags)
		, m_dstAccessFlags(dstAccessFlags)
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

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_BUFFERBARRIER);
		{
			/*
			if (m_ptrBuffer) {
				((CVKStorageBuffer*)m_ptrBuffer.GetPointer())->PipelineBarrier(m_vkCommandBuffer, m_srcAccessFlags, m_dstAccessFlags, 0, m_ptrBuffer->GetSize());
			}
			*/
		}
	}


private:
	CGfxStorageBufferPtr m_ptrBuffer;
	GfxAccessFlags m_srcAccessFlags;
	GfxAccessFlags m_dstAccessFlags;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
