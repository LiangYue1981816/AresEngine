#pragma once
#include "GLES3Renderer.h"


class CGLES3MaterialManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Material;


private:
	CGLES3MaterialManager(void);
	virtual ~CGLES3MaterialManager(void);


private:
	CGLES3Material* Get(uint32_t name);
	CGLES3Material* Create(uint32_t name);
	CGLES3Material* Create(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom);
	CGLES3Material* Create(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel, int numLevels);
	void Destroy(CGLES3Material* pMaterial);


private:
	eastl::unordered_map<uint32_t, CGLES3Material*> m_pMaterials;
};
