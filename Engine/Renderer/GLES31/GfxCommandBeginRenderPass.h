#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBeginRenderPass : public CGfxCommandBase
{
public:
	CGfxCommandBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
		: m_ptrFrameBuffer(ptrFrameBuffer)
	{

	}
	virtual ~CGfxCommandBeginRenderPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS, "CommandBeginRenderPass");
		{
			if (m_ptrFrameBuffer.IsValid()) {
				m_ptrFrameBuffer->Bind();
			}
			else {
				GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
