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
				((CGLES3MeshDraw *)m_ptrMeshDraw.GetPointer())->Bind();
				glDrawElementsIndirect(GL_TRIANGLES, CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), (const void *)0);
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
