#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindIndexBuffer : public CGfxCommandBase
{
public:
	CVKCommandBindIndexBuffer(VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndexBuffer(ptrIndexBuffer)
	{
		Execute();
	}
	virtual ~CVKCommandBindIndexBuffer(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrIndexBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INDEXBUFFER);
		{
			((CVKIndexBuffer*)m_ptrIndexBuffer.GetPointer())->Bind(m_vkCommandBuffer);
		}
	}


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
