#pragma once
#include "VKRenderer.h"


class CVKTexture : public CGfxResource
{
	friend class CVKDevice;
	friend class CVKTexture2D;


private:
	CVKTexture(CVKDevice *pDevice, VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageUsageFlags usage);
	virtual ~CVKTexture(void);
	virtual void Release(void);

private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);


private:
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKTexture> CVKTexturePtr;
