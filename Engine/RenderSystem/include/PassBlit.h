#pragma once
#include "PreHeader.h"


class CALL_API CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


protected:
	CPassBlit(const char* szMaterialFileName, CRenderSystem* pRenderSystem);
	virtual ~CPassBlit(void);


private:
	virtual void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) {}
	static  void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer, void* param);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterial;

protected:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;

protected:
	CGfxRenderQueue* m_pRenderQueue;
};
