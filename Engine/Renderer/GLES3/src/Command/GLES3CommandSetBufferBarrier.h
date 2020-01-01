#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetBufferBarrier : public CGfxCommandBase
{
public:
	CGLES3CommandSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
		: m_ptrBuffer(ptrBuffer)
		, m_srcAccessFlags(srcAccessFlags)
		, m_dstAccessFlags(dstAccessFlags)
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
				glMemoryBarrier(CGLES3Helper::TranslateBarrier(m_dstAccessFlags));
			}
		}
	}


private:
	CGfxStorageBufferPtr m_ptrBuffer;
	GfxAccessFlags m_srcAccessFlags;
	GfxAccessFlags m_dstAccessFlags;
};
