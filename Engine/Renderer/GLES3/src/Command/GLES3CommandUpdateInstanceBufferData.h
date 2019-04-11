#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUpdateInstanceBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandUpdateInstanceBuffer(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMeshDraw(ptrMeshDraw)
	{
		m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
	}
	virtual ~CGLES3CommandUpdateInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UPDATE_INSTANCEBUFFER, "CommandUpdateInstanceBuffer");
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
