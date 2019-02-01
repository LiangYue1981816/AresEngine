#pragma once
#include "VKRenderer.h"


class CVKRenderTextureManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKRenderTexture;


private:
	CVKRenderTextureManager(CVKDevice *pDevice);
	virtual ~CVKRenderTextureManager(void);


private:
	CVKRenderTexture* Get(uint32_t name);
	CVKRenderTexture* Create(uint32_t name);
	void Destroy(CVKRenderTexture *pRenderTexture);


private:
	eastl::unordered_map<uint32_t, CVKRenderTexture*> m_pRenderTextures;

private:
	CVKDevice *m_pDevice;
};
