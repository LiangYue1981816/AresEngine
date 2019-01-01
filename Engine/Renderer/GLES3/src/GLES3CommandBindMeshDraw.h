#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMeshDraw : public CGfxCommandBase
{
public:
	CGLES3CommandBindMeshDraw(const CGfxMeshDrawPtr &ptrMeshDraw)
		: m_ptrMeshDraw(ptrMeshDraw)
	{

	}
	virtual ~CGLES3CommandBindMeshDraw(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESHDRAW, "CommandBindMeshDraw");
		{
			if (m_ptrMeshDraw.IsValid()) {
				((CGLES3MeshDraw *)m_ptrMeshDraw.GetPointer())->Bind();
			}
			else {
				GLBindVertexArray(0);
				GLBindBuffer(GL_ARRAY_BUFFER, 0);
				GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#if GLES_VER == 310
				GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
#endif
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
};
