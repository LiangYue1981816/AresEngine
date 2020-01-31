#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandSetInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer, const uint8_t* pBuffer, uint32_t size)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrInstanceBuffer(ptrInstanceBuffer)
		, m_buffer(pBuffer, pBuffer + size)
	{

	}
	virtual ~CVKCommandSetInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrInstanceBuffer);
		ASSERT(m_vkCommandBuffer);

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

private:
	VkCommandBuffer m_vkCommandBuffer;
};
