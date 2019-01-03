#pragma once
#include "VKRenderer.h"


class CVKRenderTexture : public CGfxRenderTexture
{
	friend class CVKRenderTextureManager;


private:
	CVKRenderTexture(CVKDevice *pDevice, CVKRenderTextureManager *pManager, uint32_t name);
	virtual ~CVKRenderTexture(void);
	virtual void Release(void);


public:
	bool Create(GfxPixelFormat pixelFormat, int width, int height, int samples = 1);
	void Destroy(void);

private:
	bool CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format);
	bool CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);


private:
	CVKImagePtr m_ptrImage;
	VkImageView m_vkImageView;

private:
	CVKDevice *m_pDevice;

private:
	CVKRenderTextureManager *m_pManager;
};
