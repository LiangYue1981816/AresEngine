#pragma once
#include "VKRenderer.h"


class CVKMeshManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDevice;
	friend class CVKMesh;


private:
	CVKMeshManager(CVKDevice *pDevice);
	virtual ~CVKMeshManager(void);


private:
	CVKDevice* GetDevice(void) const;

private:
	bool IsHave(uint32_t name);
	CVKMesh* Create(uint32_t name);
	CVKMesh* Create(const char *szFileName, uint32_t vertexBinding);
	void Destroy(CGfxMesh *pMesh);


private:
	eastl::unordered_map<uint32_t, CGfxMesh*> m_pMeshs;

private:
	CVKDevice *m_pDevice;
};
