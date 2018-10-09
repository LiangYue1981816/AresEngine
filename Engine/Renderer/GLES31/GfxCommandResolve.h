#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandResolve : public CGfxCommandBase
{
public:
	CGfxCommandResolve(const CGfxFrameBufferPtr &ptrFrameBufferSrc, const CGfxFrameBufferPtr &ptrFrameBufferDst)
		: m_ptrFrameBufferSrc(ptrFrameBufferSrc)
		, m_ptrFrameBufferDst(ptrFrameBufferDst)
	{

	}
	virtual ~CGfxCommandResolve(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_RESOLVE, "CommandResolve");
		{
			if (m_ptrFrameBufferSrc.IsValid() && m_ptrFrameBufferDst.IsValid() && m_ptrFrameBufferSrc != m_ptrFrameBufferDst) {
				GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_ptrFrameBufferSrc->GetFBO());
				GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ptrFrameBufferDst->GetFBO());
				glBlitFramebuffer(
					0, 0, m_ptrFrameBufferSrc->GetWidth() - 1, m_ptrFrameBufferSrc->GetHeight() - 1, 
					0, 0, m_ptrFrameBufferDst->GetWidth() - 1, m_ptrFrameBufferDst->GetHeight() - 1, 
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBufferSrc;
	CGfxFrameBufferPtr m_ptrFrameBufferDst;
};
