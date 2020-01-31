#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetIndexBuffer : public CGfxCommandBase
{
public:
	CVKCommandSetIndexBuffer(VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndexBuffer(ptrIndexBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CVKCommandSetIndexBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrIndexBuffer);
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_INDEXBUFFER, "CommandSetIndexBuffer");
		{
			if (m_buffer.empty() == false) {
				m_ptrIndexBuffer->BufferData(0, m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
	eastl::vector<uint8_t> m_buffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
