#pragma once
#include "VKRenderer.h"


class CVKTextureCubeMapManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTextureCubeMap;


private:
	CVKTextureCubeMapManager(CVKDevice *pDevice);
	virtual ~CVKTextureCubeMapManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHave(uint32_t name);
	CVKTextureCubeMap* Create(uint32_t name);
	CVKTextureCubeMap* Create(const char *szFileName);
	void Destroy(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};
