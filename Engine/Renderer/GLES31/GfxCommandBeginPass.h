#pragma once
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
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BEGIN_PASS, "CommandBeginPass");
		{
			if (m_ptrFrameBuffer.IsValid()) {
				m_ptrFrameBuffer->Bind();
			}
			else {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
};
