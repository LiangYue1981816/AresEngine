#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
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
		if (m_ptrMesh.IsValid()) {
			m_ptrMesh->ClearInstance();
			m_ptrMesh->SetInstance(m_mtxTransforms);
			m_ptrMesh->UpdateInstance();
			m_ptrMesh->Bind();
		}
		else {
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}


private:
	CGfxMeshPtr m_ptrMesh;
	eastl::vector<glm::mat4> m_mtxTransforms;
};
