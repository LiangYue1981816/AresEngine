#pragma once
#include "VKRenderer.h"


class CVKSampler : public CGfxSampler
{
	friend class CVKSamplerManager;


private:
	CVKSampler(CVKDevice *pDevice, int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);
	virtual ~CVKSampler(void);


public:
	HANDLE GetSampler(void) const;


private:
	VkSampler m_vkSampler;

private:
	CVKDevice *m_pDevice;
};
