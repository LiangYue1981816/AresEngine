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
	CGLES3Mesh* Create(const char *szFileName, uint32_t instanceFormat);
	CGLES3Mesh* Create(uint32_t name, const char *szFileName, uint32_t instanceFormat);
	CGLES3Mesh* Create(uint32_t name);
	void Destroy(CGfxMesh *pMesh);


private:
	eastl::unordered_map<uint32_t, CGfxMesh*> m_pMeshs;
};
