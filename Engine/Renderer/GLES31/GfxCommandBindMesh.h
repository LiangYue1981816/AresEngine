#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"


class CGfxCommandBindMesh : public CGfxCommandBase
{
public:
	CGfxCommandBindMesh(CGfxMesh *pMesh)
		: m_pMesh(pMesh)
	{
		if (m_pMesh) {
			m_pMesh->Retain();
		}
	}
	CGfxCommandBindMesh(CGfxMesh *pMesh, eastl::vector<glm::mat4> &mtxTransforms)
		: m_pMesh(pMesh)
		, m_mtxTransforms(mtxTransforms)
	{
		if (m_pMesh) {
			m_pMesh->Retain();
		}
	}
	virtual ~CGfxCommandBindMesh(void)
	{
		if (m_pMesh) {
			m_pMesh->Release();
		}
	}

public:
	virtual void Execute(void) const
	{
		if (m_pMesh) {
			if (m_mtxTransforms.size()) {
				m_pMesh->ClearInstance();

				for (GLuint index = 0; index < m_mtxTransforms.size(); index++) {
					m_pMesh->AddInstance(m_mtxTransforms[index]);
				}
			}

			Renderer()->BindMesh(m_pMesh);
		}
		else {
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}


private:
	CGfxMesh *m_pMesh;
	eastl::vector<glm::mat4> m_mtxTransforms;
};
