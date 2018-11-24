#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandSetDrawIndirectBufferData : public CGfxCommandBase
{
public:
	CGLES3CommandSetDrawIndirectBufferData(const CGfxMeshPtr &ptrMesh, const int indexDraw, int instanceCount)
		: m_ptrMesh(ptrMesh)
		, m_indexDraw(indexDraw)
		, m_instanceCount(instanceCount)
	{

	}
	virtual ~CGLES3CommandSetDrawIndirectBufferData(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_DRAWINDIRECTBUFFERDATA, "CommandSetDrawIndirectBufferData");
		{
			if (m_ptrMesh.IsValid()) {
				m_ptrMesh->DrawIndirectBufferData((int)m_indexDraw, m_instanceCount);
			}
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	uintptr_t m_indexDraw;
	int m_instanceCount;
};
