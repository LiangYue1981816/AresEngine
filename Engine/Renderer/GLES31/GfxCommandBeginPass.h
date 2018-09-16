#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"


class CGfxCommandBeginPass : public CGfxCommandBase
{
public:
	CGfxCommandBeginPass(CGfxFrameBuffer *pFrameBuffer)
		: m_pFrameBuffer(pFrameBuffer)
	{
		if (m_pFrameBuffer) {
			m_pFrameBuffer->Retain();
		}
	}
	virtual ~CGfxCommandBeginPass(void)
	{
		if (m_pFrameBuffer) {
			m_pFrameBuffer->Release();
		}
	}

public:
	virtual void Execute(void) const
	{
		if (m_pFrameBuffer) {
			Renderer()->BindFrameBuffer(m_pFrameBuffer);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}


private:
	CGfxFrameBuffer *m_pFrameBuffer;
};
