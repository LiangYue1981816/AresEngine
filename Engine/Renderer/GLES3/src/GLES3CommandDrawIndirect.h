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
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INDIRECT, "CommandDrawIndirect");
		{
			if (m_ptrMeshDraw.IsValid()) {
				glDrawElementsIndirect(
					CGLES3Helper::TranslatePrimitiveTopology(GLES3Renderer()->GetCurrentPipelineGraphics()->GetPipelineState().topology), 
					CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()),
					(const void *)0);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
