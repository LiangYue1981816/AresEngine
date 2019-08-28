#pragma once
#include "RenderSystem.h"


class CALL_API CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


protected:
	CPassBlit(const char* szMaterialFileName);
	virtual ~CPassBlit(void);


protected:
	void CmdBlit(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterial;

private:
	CGfxRenderQueue* m_pRenderQueue;
};
