#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetIndexBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandSetIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_ptrIndexBuffer(ptrIndexBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CGLES3CommandSetIndexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrIndexBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_INDEXBUFFER);
		{
			if (m_buffer.empty() == false) {
				m_ptrIndexBuffer->BufferData(0, m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
	eastl::vector<uint8_t> m_buffer;
};
