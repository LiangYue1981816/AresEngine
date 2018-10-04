#pragma once
#include "GfxRenderer.h"


class CGfxMaterialManager
{
	friend class CGfxRenderer;


private:
	CGfxMaterialManager(void);
	virtual ~CGfxMaterialManager(void);


private:
	CGfxMaterial* LoadMaterial(const char *szFileName);
	void DestroyMaterial(CGfxMaterial *pMaterial);


private:
	eastl::unordered_map<GLuint, CGfxMaterial*> m_pMaterials;
};
