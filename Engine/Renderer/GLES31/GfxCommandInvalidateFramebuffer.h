#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandInvalidateFramebuffer : public CGfxCommandBase
{
public:
	CGfxCommandInvalidateFramebuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexSubPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubPass(indexSubPass)
	{

	}
	virtual ~CGfxCommandInvalidateFramebuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_INVALIDATE_FRAMEBUFFER, "CommandInvalidateFramebuffer");
		{
			if (m_ptrFrameBuffer.IsValid() && m_ptrRenderPass.IsValid()) {
				m_ptrFrameBuffer->InvalidateFramebuffer(m_ptrRenderPass->GetAttachments(), m_ptrRenderPass->GetSubPass(m_indexSubPass));
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubPass;
};
