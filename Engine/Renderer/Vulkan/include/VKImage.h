#pragma once
#include "VKRenderer.h"


class CVKImage : public CGfxResource
{
	friend class CVKDevice;
	friend class CVKTexture2D;


private:
	CVKImage(CVKDevice *pDevice, VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);
	virtual ~CVKImage(void);
	virtual void Release(void);

private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;


private:
	VkImage m_vkImage;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKImage> CVKImagePtr;
