#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandEndPass : public CGfxCommandBase
{
public:
	CGfxCommandEndPass(CGfxFrameBuffer *pFrameBuffer)
		: m_pFrameBuffer(pFrameBuffer)
	{
		if (m_pFrameBuffer) {
			m_pFrameBuffer->Retain();
		}
	}
	virtual ~CGfxCommandEndPass(void)
	{
		if (m_pFrameBuffer) {
			m_pFrameBuffer->Release();
		}
	}

public:
	virtual void Execute(void) const
	{
		if (m_pFrameBuffer) {
			Renderer()->InvalidateFramebuffer(m_pFrameBuffer);
		}
	}


private:
	CGfxFrameBuffer *m_pFrameBuffer;
};
