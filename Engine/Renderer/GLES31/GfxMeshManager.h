#pragma once
#include "GfxRenderer.h"


class CGfxMeshManager
{
	friend class CGfxRenderer;


private:
	CGfxMeshManager(void);
	virtual ~CGfxMeshManager(void);


private:
	CGfxMesh* LoadMesh(const char *szFileName);
	CGfxMesh* CreateMesh(uint32_t name);
	void DestroyMesh(CGfxMesh *pMesh);


private:
	eastl::unordered_map<uint32_t, CGfxMesh*> m_pMeshs;
};
