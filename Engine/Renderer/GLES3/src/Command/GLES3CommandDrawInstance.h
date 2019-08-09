#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxMeshDrawPtr ptrMeshDraw)
		: m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_ptrMeshDraw);
		ASSERT(m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetVertexBinding(), m_ptrMeshDraw->GetVertexFormat()) &&
				m_pPipelineGraphics->IsCompatibleVertexFormat(m_ptrMeshDraw->GetInstanceBinding(), m_ptrMeshDraw->GetInstanceFormat())) {
				glDrawElementsInstanced(GL_TRIANGLES, m_ptrMeshDraw->GetIndexCount(), CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), (const void*)m_ptrMeshDraw->GetIndexOffset(), m_ptrMeshDraw->GetInstanceCount());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
