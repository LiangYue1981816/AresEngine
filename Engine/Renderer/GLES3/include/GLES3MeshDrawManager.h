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
	bool IsHave(uint32_t name);
	CGfxMeshDraw* Create(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding);
	void Destroy(CGfxMeshDraw *pMeshDraw);


private:
	eastl::unordered_map<uint32_t, CGfxMeshDraw*> m_pMeshDraws;
};
