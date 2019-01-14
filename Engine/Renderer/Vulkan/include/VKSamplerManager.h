#pragma once
#include "VKRenderer.h"


class CVKSamplerManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKSampler;


private:
	CVKSamplerManager(CVKDevice *pDevice);
	virtual ~CVKSamplerManager(void);


private:
	CVKSampler* Create(int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);


private:
	eastl::unordered_map<uint32_t, CGfxSampler*> m_pSamplers;

private:
	CVKDevice *m_pDevice;
};
