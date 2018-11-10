#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMesh : public CGfxCommandBase
{
public:
	CGLES3CommandBindMesh(CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size)
		: m_ptrMesh(ptrMesh)
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
				m_ptrMesh->InstanceBufferData(m_buffer.size(), m_buffer.data());
				m_ptrMesh->Bind(nullptr);
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
