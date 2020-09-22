#pragma once
#include "VKRenderer.h"


class CVKSampler : public CGfxSampler
{
	friend class CVKSamplerManager;


private:
	CVKSampler(CVKDevice* pDevice, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);
	virtual ~CVKSampler(void);


public:
	VkSampler GetSampler(void) const;


private:
	VkSampler m_vkSampler;

private:
	CVKDevice* m_pDevice;
};
