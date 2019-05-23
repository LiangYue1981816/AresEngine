#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawIndirect : public CGfxCommandBase
{
public:
	CGLES3CommandDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandDrawIndirect(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrMeshDraw);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (CGLES3PipelineGraphics* pPipeline = GLES3Renderer()->GetCurrentPipelineGraphics()) {
				if (pPipeline->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
					pPipeline->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
					((CGLES3MeshDraw*)m_ptrMeshDraw.GetPointer())->Bind();
					glDrawElementsIndirect(GL_TRIANGLES, CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), (const void*)0);
				}
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
