#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetVertexBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandSetVertexBuffer(const CGfxVertexBufferPtr ptrVertexBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_ptrVertexBuffer(ptrVertexBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CGLES3CommandSetVertexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrVertexBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_VERTEXBUFFER, "CommandSetVertexBuffer");
		{
			if (m_buffer.empty() == false) {
				m_ptrVertexBuffer->BufferData(0, m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	eastl::vector<uint8_t> m_buffer;
};
