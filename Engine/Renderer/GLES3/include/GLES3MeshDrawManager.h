#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDrawManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3MeshDraw;


private:
	CGLES3MeshDrawManager(void);
	virtual ~CGLES3MeshDrawManager(void);


private:
	CGLES3MeshDraw* Get(uint32_t name);
	CGLES3MeshDraw* Create(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding);
	void Destroy(CGLES3MeshDraw* pMeshDraw);


private:
	eastl::unordered_map<uint32_t, CGLES3MeshDraw*> m_pMeshDraws;
};
