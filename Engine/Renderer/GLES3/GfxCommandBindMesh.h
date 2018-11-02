#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindMesh : public CGfxCommandBase
{
public:
	CGfxCommandBindMesh(const CGfxMeshPtr &ptrMesh)
		: m_ptrMesh(ptrMesh)
	{

	}
	CGfxCommandBindMesh(const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMesh(ptrMesh)
	{
		m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
	}
	virtual ~CGfxCommandBindMesh(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MESH, "CommandBindMesh");
		{
			if (m_ptrMesh.IsValid()) {
				m_ptrMesh->SetInstance(m_buffer.data(), m_buffer.size());
				m_ptrMesh->UpdateInstance();
				m_ptrMesh->Bind();
			}
			else {
				GLBindVertexArray(0);
				GLBindBuffer(GL_ARRAY_BUFFER, 0);
				GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	eastl::vector<uint8_t> m_buffer;
};
