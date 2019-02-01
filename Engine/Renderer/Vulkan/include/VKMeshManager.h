#pragma once
#include "VKRenderer.h"


class CVKMeshManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKMesh;


private:
	CVKMeshManager(CVKDevice *pDevice);
	virtual ~CVKMeshManager(void);


private:
	CVKMesh* Get(uint32_t name);
	CVKMesh* Create(uint32_t name);
	CVKMesh* Create(const char *szFileName, uint32_t vertexBinding);
	void Destroy(CVKMesh *pMesh);


private:
	eastl::unordered_map<uint32_t, CVKMesh*> m_pMeshs;

private:
	CVKDevice *m_pDevice;
};
