#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandDrawInstance : public CGfxCommandBase
{
public:
	CGLES3CommandDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandDrawInstance(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_DRAW_INSTANCE, "CommandDrawInstance");
		{
			if (m_ptrMeshDraw.IsValid()) {
				((CGLES3MeshDraw *)m_ptrMeshDraw.GetPointer())->Bind();
				glDrawElementsInstanced(GL_TRIANGLES, m_ptrMeshDraw->GetIndexCount(), CGLES3Helper::TranslateIndexType(m_ptrMeshDraw->GetIndexType()), (const void *)m_ptrMeshDraw->GetIndexOffset(), m_ptrMeshDraw->GetInstanceCount());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
