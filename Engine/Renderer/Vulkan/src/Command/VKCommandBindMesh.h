#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindMesh : public CGfxCommandBase
{
public:
	CVKCommandBindMesh(VkCommandBuffer vkCommandBuffer, const CGfxMeshPtr ptrMesh)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrMesh(ptrMesh)
	{
		Execute();
	}
	virtual ~CVKCommandBindMesh(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrMesh);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESH, "CommandBindMesh");
		{
			((CVKMesh*)m_ptrMesh.GetPointer())->Bind(m_vkCommandBuffer);
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
