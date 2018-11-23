#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMesh : public CGfxCommandBase
{
public:
	CGLES3CommandBindMesh(const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMesh(ptrMesh)
		, m_indexDraw(indexDraw)
	{
		m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
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
				m_ptrMesh->InstanceBufferData((int)m_indexDraw, m_buffer.size(), m_buffer.data());
				m_ptrMesh->DrawIndirectBufferData((int)m_indexDraw, m_buffer.size() / GetInstanceStride(m_ptrMesh->GetInstanceFormat()));
				m_ptrMesh->Bind((void *)m_indexDraw);
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
	eastl::vector<uint8_t> m_buffer;
};
