#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMeshDraw : public CGfxCommandBase
{
public:
	CGLES3CommandBindMeshDraw(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandBindMeshDraw(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrMeshDraw);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESHDRAW, "CommandBindMeshDraw");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
				m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				((CGLES3MeshDraw*)m_ptrMeshDraw.GetPointer())->Bind();
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
