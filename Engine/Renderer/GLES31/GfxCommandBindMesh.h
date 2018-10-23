#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindMesh : public CGfxCommandBase
{
public:
	CGfxCommandBindMesh(const CGfxMeshPtr &ptrMesh, const eastl::vector<glm::mat4> &mtxTransforms)
		: m_ptrMesh(ptrMesh)
		, m_mtxTransforms(mtxTransforms)
	{

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
				m_ptrMesh->SetInstance(m_mtxTransforms);
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
	eastl::vector<glm::mat4> m_mtxTransforms;
};
