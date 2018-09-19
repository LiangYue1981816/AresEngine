#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandEndPass : public CGfxCommandBase
{
public:
	CGfxCommandEndPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
		: m_ptrFrameBuffer(ptrFrameBuffer)
	{

	}
	virtual ~CGfxCommandEndPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		if (m_ptrFrameBuffer.IsValid()) {
			m_ptrFrameBuffer->InvalidateFramebuffer();
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
