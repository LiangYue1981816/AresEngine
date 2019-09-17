#pragma once
#include "PreHeader.h"


class CALL_API CPassBlit : public CPassBase
{
	friend class CCamera;
	friend class CRenderSystem;


protected:
	CPassBlit(const char* szMaterialFileName, CRenderSystem* pRenderSystem);
	virtual ~CPassBlit(void);


protected:
	virtual void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) {}
	static  void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer, void* param);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterial;

protected:
	CGfxRenderQueue* m_pRenderQueue;
};
