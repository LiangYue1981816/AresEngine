#pragma once
#include "GLES3Renderer.h"


class CGLES3MaterialManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Material;


private:
	CGLES3MaterialManager(void);
	virtual ~CGLES3MaterialManager(void);


private:
	CGLES3Material* LoadMaterial(const char *szFileName);
	void DestroyMaterial(CGLES3Material *pMaterial);


private:
	eastl::unordered_map<uint32_t, CGLES3Material*> m_pMaterials;
};
