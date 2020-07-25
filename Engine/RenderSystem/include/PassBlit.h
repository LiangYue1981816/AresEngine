#pragma once
#include "PreHeader.h"


class CPassBlit : public CPassBase
{
	friend class CCamera;
	friend class CRenderQueue;
	friend class CRenderSystem;


protected:
	CPassBlit(const char* szMaterialFileName, CRenderSystem* pRenderSystem);
	virtual ~CPassBlit(void);


protected:
	virtual void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) {}
	static  void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer, void* param);


protected:
	CRenderQueue* m_pRenderQueue;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterial;
};
