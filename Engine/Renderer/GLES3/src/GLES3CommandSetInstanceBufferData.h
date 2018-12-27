#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetInstanceBufferData : public CGfxCommandBase
{
public:
	CGLES3CommandSetInstanceBufferData(const CGfxMeshDrawPtr &ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMeshDraw(ptrMeshDraw)
		, m_buffer(pInstanceBuffer, pInstanceBuffer + size)
	{

	}
	virtual ~CGLES3CommandSetInstanceBufferData(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_INSTANCEBUFFERDATA, "CommandSetInstanceBufferData");
		{
			if (m_ptrMeshDraw.IsValid()) {
				m_ptrMeshDraw->InstanceBufferData(m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	eastl::vector<uint8_t> m_buffer;
};
