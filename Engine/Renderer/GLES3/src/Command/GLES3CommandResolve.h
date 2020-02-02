#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandResolve : public CGfxCommandBase
{
public:
	CGLES3CommandResolve(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{

	}
	virtual ~CGLES3CommandResolve(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrFrameBuffer);
		ASSERT(m_ptrRenderPass);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_RESOLVE);
		{
			((CGLES3FrameBuffer*)m_ptrFrameBuffer.GetPointer())->Resolve(m_ptrRenderPass->GetAttachments(), m_ptrRenderPass->GetSubpass(m_indexSubpass));
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
