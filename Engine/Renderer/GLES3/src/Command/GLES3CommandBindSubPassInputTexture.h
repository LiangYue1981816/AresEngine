#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindSubpassInputTexture : public CGfxCommandBase
{
public:
	CGLES3CommandBindSubpassInputTexture(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{

	}
	virtual ~CGLES3CommandBindSubpassInputTexture(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_SUBPASSINPUTTEXTURE, "CommandBindSubpassInputTexture");
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
