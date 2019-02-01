#pragma once
#include "VKRenderer.h"


class CVKMaterialManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKMaterial;


private:
	CVKMaterialManager(CVKDevice *pDevice);
	virtual ~CVKMaterialManager(void);


private:
	CVKMaterial* Get(uint32_t name);
	CVKMaterial* Create(uint32_t name);
	CVKMaterial* Create(const char *szFileName, uint32_t vertexBinding, uint32_t instanceBinding);
	void Destroy(CVKMaterial *pMaterial);


private:
	eastl::unordered_map<uint32_t, CVKMaterial*> m_pMaterials;

private:
	CVKDevice *m_pDevice;
};
