#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetInstanceBufferData : public CGfxCommandBase
{
public:
	CGLES3CommandSetInstanceBufferData(const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMesh(ptrMesh)
		, m_indexDraw(indexDraw)
	{
		m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
	}
	virtual ~CGLES3CommandSetInstanceBufferData(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_INSTANCEBUFFERDATA, "CommandSetInstanceBufferData");
		{
			if (m_ptrMesh.IsValid()) {
				m_ptrMesh->InstanceBufferData(m_indexDraw, m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	int m_indexDraw;

	eastl::vector<uint8_t> m_buffer;
};
