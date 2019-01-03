#pragma once
#include "VKRenderer.h"


class CVKRenderTextureManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDevice;
	friend class CVKRenderTexture;


private:
	CVKRenderTextureManager(CVKDevice *pDevice);
	virtual ~CVKRenderTextureManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHaveRenderTexture(uint32_t name);
	CVKRenderTexture* CreateRenderTexture(uint32_t name);
	void Destroy(CGfxRenderTexture *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxRenderTexture*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};
