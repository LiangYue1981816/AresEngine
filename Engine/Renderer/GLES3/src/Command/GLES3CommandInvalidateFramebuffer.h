#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandInvalidateFramebuffer : public CGfxCommandBase
{
public:
	CGLES3CommandInvalidateFramebuffer(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{

	}
	virtual ~CGLES3CommandInvalidateFramebuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrFrameBuffer);
		ASSERT(m_ptrRenderPass);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_INVALIDATE_FRAMEBUFFER, "CommandInvalidateFramebuffer");
		{
			((CGLES3FrameBuffer*)m_ptrFrameBuffer.GetPointer())->InvalidateFramebuffer(m_ptrRenderPass->GetAttachments(), m_ptrRenderPass->GetSubpass(m_indexSubpass));
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
