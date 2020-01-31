#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetInstanceBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandSetInstanceBuffer(const CGfxInstanceBufferPtr ptrInstanceBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_ptrInstanceBuffer(ptrInstanceBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CGLES3CommandSetInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrInstanceBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_INSTANCEBUFFER, "CommandSetInstanceBuffer");
		{
			if (m_buffer.empty() == false) {
				m_ptrInstanceBuffer->BufferData(m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	eastl::vector<uint8_t> m_buffer;
};
