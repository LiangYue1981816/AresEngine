#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawElements : public CGfxCommandBase
{
public:
	CGLES3CommandDrawElements(const CGfxMeshDrawPtr ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandDrawElements(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_ELEMENTS, "CommandDrawElements");
		{
			if (m_ptrMeshDraw.IsValid()) {
				glDrawElements(
					CGLES3Helper::TranslatePrimitiveTopology(GLES3Renderer()->GetCurrentPipelineGraphics()->GetPipelineState().topology), 
					m_ptrMeshDraw->GetIndexCount(), 
					CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), 
					(const void *)m_ptrMeshDraw->GetIndexOffset());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
