#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMesh : public CGfxCommandBase
{
public:
	CGLES3CommandBindMesh(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshPtr ptrMesh)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrMesh(ptrMesh)
	{

	}
	virtual ~CGLES3CommandBindMesh(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrMesh);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESH, "CommandBindMesh");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMesh->GetVertexBufferPtr()->GetVertexBinding(), m_ptrMesh->GetVertexBufferPtr()->GetVertexFormat())) {
				((CGLES3Mesh*)m_ptrMesh.GetPointer())->Bind();
			}
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
