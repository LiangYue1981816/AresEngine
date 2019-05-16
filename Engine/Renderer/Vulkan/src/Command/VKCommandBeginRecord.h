#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBeginRecord : public CGfxCommandBase
{
public:
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer)
		: m_indexSubpass(0)
	{
		Execute(vkCommandBuffer, true);
	}
	CVKCommandBeginRecord(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{
		Execute(vkCommandBuffer, false);
	}
	virtual ~CVKCommandBeginRecord(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer, bool bMainCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RECORD, "CommandBeginRecord");
		{

		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
