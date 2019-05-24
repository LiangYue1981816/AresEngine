#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawIndirect : public CGfxCommandBase
{
public:
	CGLES3CommandDrawIndirect(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrMeshDraw);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
				m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				((CGLES3MeshDraw*)m_ptrMeshDraw.GetPointer())->Bind();
				glDrawElementsIndirect(GL_TRIANGLES, CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), (const void*)0);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;

private:
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
