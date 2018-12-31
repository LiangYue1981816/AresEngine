#pragma once
#include "VKRenderer.h"


class CVKImage : public CGfxResource
{
	friend class CVKDevice;
	friend class CVKTexture2D;


private:
	CVKImage(CVKDevice *pDevice, VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageUsageFlags usage);
	virtual ~CVKImage(void);
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

typedef CGfxResourcePtr<CVKImage> CVKImagePtr;
