#pragma once
#include "RenderSystem.h"


class CALL_API CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


protected:
	CPassBlit(const char* szMaterialFileName);
	virtual ~CPassBlit(void);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterial;

protected:
	CGfxRenderQueue* m_pRenderQueue;
	CGfxUniformCamera* m_pCameraUniform;
};
