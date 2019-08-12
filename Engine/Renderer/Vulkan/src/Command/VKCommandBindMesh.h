#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBindMesh : public CGfxCommandBase
{
public:
	CVKCommandBindMesh(VkCommandBuffer vkCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshPtr ptrMesh)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_pPipelineGraphics((CVKPipelineGraphics*)pPipelineGraphics)
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
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESH, "CommandBindMesh");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMesh->GetVertexBuffer()->GetVertexBinding(), m_ptrMesh->GetVertexBuffer()->GetVertexFormat())) {
				((CVKMesh*)m_ptrMesh.GetPointer())->Bind(m_vkCommandBuffer);
			}
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	CVKPipelineGraphics* m_pPipelineGraphics;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
