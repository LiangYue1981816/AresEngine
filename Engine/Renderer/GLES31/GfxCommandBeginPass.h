#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"


class CGfxCommandBeginPass : public CGfxCommandBase
{
public:
	CGfxCommandBeginPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
		: m_ptrFrameBuffer(ptrFrameBuffer)
	{

	}
	virtual ~CGfxCommandBeginPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		if (m_ptrFrameBuffer.IsValid()) {
			Renderer()->BindFrameBuffer(m_ptrFrameBuffer);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
