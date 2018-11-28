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
	CGfxMaterial* Create(const char *szFileName);
	void Destroy(CGfxMaterial *pMaterial);


private:
	eastl::unordered_map<uint32_t, CGfxMaterial*> m_pMaterials;
};
