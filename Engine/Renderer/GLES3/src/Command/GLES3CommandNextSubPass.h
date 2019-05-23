#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandNextSubPass : public CGfxCommandBase
{
public:
	CGLES3CommandNextSubPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{

	}
	virtual ~CGLES3CommandNextSubPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrRenderPass);
		ASSERT(m_ptrFrameBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_NEXT_SUBPASS, "CommandNextSubPass");
		{

		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
