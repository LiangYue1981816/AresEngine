#pragma once
#include "VKRenderer.h"


class CVKTexture
{
	friend class CVKDevice;
	friend class CVKTextureManager;


protected:
	CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager);
	virtual ~CVKTexture(void);


public:
	bool Create(VkImageView vkImageView);
	void Destroy(void);

public:
	VkImageView GetImageView(void) const;


protected:
	eastl::unordered_map<int, VkBufferImageCopy> m_transferRegions;
	eastl::unordered_map<int, eastl::vector<uint8_t>> m_transferBuffer;

protected:
	bool m_bExtern;
	CVKImagePtr m_ptrImage;
	VkImageView m_vkImageView;

protected:
	CVKDevice *m_pDevice;

protected:
	CVKTextureManager *m_pManager;
};
