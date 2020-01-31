#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetVertexBuffer : public CGfxCommandBase
{
public:
	CVKCommandSetVertexBuffer(VkCommandBuffer vkCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrVertexBuffer(ptrVertexBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CVKCommandSetVertexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrVertexBuffer);
		ASSERT(m_vkCommandBuffer);

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

private:
	VkCommandBuffer m_vkCommandBuffer;
};
