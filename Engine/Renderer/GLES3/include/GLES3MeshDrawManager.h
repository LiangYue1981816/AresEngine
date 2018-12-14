#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDrawManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Mesh;


private:
	CGLES3MeshDrawManager(void);
	virtual ~CGLES3MeshDrawManager(void);


private:
	CGfxMeshDraw* Create(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceBinding, uint32_t instanceFormat);
	void Destroy(CGfxMeshDraw *pMeshDraw);


private:
	eastl::unordered_map<CGfxMeshDraw*, CGfxMeshDraw*> m_pMeshDraws;
};
