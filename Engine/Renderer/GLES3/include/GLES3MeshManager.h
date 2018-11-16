#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Mesh;


private:
	CGLES3MeshManager(void);
	virtual ~CGLES3MeshManager(void);


private:
	CGLES3Mesh* LoadMesh(const char *szFileName, uint32_t instanceFormat);
	CGLES3Mesh* CreateMesh(uint32_t name, uint32_t instanceFormat);
	void DestroyMesh(CGLES3Mesh *pMesh);


private:
	eastl::unordered_map<uint32_t, CGLES3Mesh*> m_pMeshs;
};
