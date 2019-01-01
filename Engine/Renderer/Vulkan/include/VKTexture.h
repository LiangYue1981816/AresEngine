#pragma once
#include "VKRenderer.h"


class CVKTexture
{
	friend class CVKDevice;
	friend class CVKTextureManager;


protected:
	CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager);
	virtual ~CVKTexture(void);


protected:
	eastl::vector<uint8_t> m_transferBuffer;
	eastl::vector<VkBufferImageCopy> m_transferRegions;

protected:
	CVKImagePtr m_ptrImage;

protected:
	CVKDevice *m_pDevice;

protected:
	CVKTextureManager *m_pManager;
};
