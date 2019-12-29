#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetBufferBarrier : public CGfxCommandBase
{
public:
	CVKCommandSetBufferBarrier(VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags, GfxPipelineStageFlags srcPipelineStageFlags, GfxPipelineStageFlags dstPipelineStageFlags)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_srcAccessFlags(srcAccessFlags)
		, m_dstAccessFlags(dstAccessFlags)
		, m_srcPipelineStageFlags(srcPipelineStageFlags)
		, m_dstPipelineStageFlags(dstPipelineStageFlags)
		, m_ptrBuffer(ptrBuffer)
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

			}
		}
	}


private:
	GfxAccessFlags m_srcAccessFlags;
	GfxAccessFlags m_dstAccessFlags;
	GfxPipelineStageFlags m_srcPipelineStageFlags;
	GfxPipelineStageFlags m_dstPipelineStageFlags;
	CGfxStorageBufferPtr m_ptrBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
