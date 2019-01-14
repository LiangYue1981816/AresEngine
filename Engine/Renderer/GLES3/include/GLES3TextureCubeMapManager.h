#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureCubeMapManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3TextureCubeMap;


private:
	CGLES3TextureCubeMapManager(void);
	virtual ~CGLES3TextureCubeMapManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3TextureCubeMap* Create(uint32_t name);
	CGLES3TextureCubeMap* Create(const char *szFileName);
	void Destroy(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextures;
};
