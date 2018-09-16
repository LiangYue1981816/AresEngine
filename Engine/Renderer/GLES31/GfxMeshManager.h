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
	void FreeMesh(CGfxMesh *pMesh);


private:
	eastl::unordered_map<GLuint, CGfxMesh*> m_pMeshs;
};
