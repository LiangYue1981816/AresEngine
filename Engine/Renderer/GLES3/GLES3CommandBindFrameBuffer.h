#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindFrameBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandBindFrameBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexSubPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubPass(indexSubPass)
	{

	}
	virtual ~CGLES3CommandBindFrameBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_FRAMEBUFFER, "CommandBindFrameBuffer");
		{
			if (m_ptrFrameBuffer.IsValid() && m_ptrRenderPass.IsValid()) {
				((CGLES3FrameBuffer *)&m_ptrFrameBuffer)->Bind(m_ptrRenderPass->GetAttachments(), m_ptrRenderPass->GetSubPass(m_indexSubPass));
			}
			else {
				GLBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubPass;
};
