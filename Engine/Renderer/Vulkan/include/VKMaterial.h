#pragma once
#include "VKRenderer.h"


class CVKMaterial : public CGfxMaterial
{
	friend class CVKRenderer;
	friend class CVKMaterialManager;


private:
	CVKMaterial(CVKDevice* pDevice, CVKMaterialManager* pManager, uint32_t name);
	CVKMaterial(CVKDevice* pDevice, CVKMaterialManager* pManager, uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom);
	virtual ~CVKMaterial(void);
	virtual void Release(void);


public:
	CGfxMaterialPass* GetPass(uint32_t name);
	CGfxMaterialPass* CreatePass(uint32_t name);
	void Destroy(void);


private:
	eastl::unordered_map<uint32_t, CVKMaterialPass*> m_pPasses;

private:
	CVKDevice* m_pDevice;

private:
	CVKMaterialManager* m_pManager;
};
