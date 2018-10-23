#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindDrawIndirectBuffer : public CGfxCommandBase
{
public:
	CGfxCommandBindDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, uint32_t instanceCount)
		: m_ptrDrawIndirectBuffer(ptrDrawIndirectBuffer)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGfxCommandBindDrawIndirectBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_DRAWINDIRECTBUFFER, "CommandBindDrawIndirectBuffer");
		{
			if (m_ptrDrawIndirectBuffer.IsValid()) {
				m_ptrDrawIndirectBuffer->Bind(m_instanceCount);
			}
			else {
				GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
			}
		}
	}


private:
	CGfxDrawIndirectBufferPtr m_ptrDrawIndirectBuffer;
	uint32_t m_instanceCount;
};
