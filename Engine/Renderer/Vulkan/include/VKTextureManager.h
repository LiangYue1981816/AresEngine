#pragma once
#include "VKRenderer.h"


class CVKTextureManager : public CGfxResourceManager
{
	friend class CVKDevice;
	friend class CVKTexture;


private:
	CVKTextureManager(CVKDevice *pDevice);
	virtual ~CVKTextureManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	eastl::unordered_map<uint32_t, CGfxTexture*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};
