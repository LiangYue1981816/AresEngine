#pragma once
#include "VKRenderer.h"


class CVKTexture2DManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTexture2D;


private:
	CVKTexture2DManager(CVKDevice *pDevice);
	virtual ~CVKTexture2DManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHave(uint32_t name);
	CVKTexture2D* Create(uint32_t name);
	CVKTexture2D* Create(const char *szFileName);
	void Destroy(CGfxTexture2D *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};
