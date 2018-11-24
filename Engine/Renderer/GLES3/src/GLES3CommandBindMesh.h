#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMesh : public CGfxCommandBase
{
public:
	CGLES3CommandBindMesh(const CGfxMeshPtr &ptrMesh, const int indexDraw)
		: m_ptrMesh(ptrMesh)
		, m_indexDraw(indexDraw)
	{

	}
	virtual ~CGLES3CommandBindMesh(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESH, "CommandBindMesh");
		{
			if (m_ptrMesh.IsValid()) {
				m_ptrMesh->Bind(m_indexDraw, nullptr);
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
	CGfxMeshPtr m_ptrMesh;
	uintptr_t m_indexDraw;
};
