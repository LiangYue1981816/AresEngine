#pragma once
#include "VKRenderer.h"


class CVKRenderTexture : public CGfxRenderTexture
{
	friend class CVKRenderTextureManager;


private:
	CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name);
	virtual ~CVKRenderTexture(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetTexture(void) const;

public:
	GfxPixelFormat GetFormat(void) const;
	GfxTextureType GetType(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetSamples(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);
	void Destroy(void);

private:
	bool CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format);
	bool CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_samples;

private:
	bool m_bExtern;
	CVKImagePtr m_ptrImage;
	VkImageView m_vkImageView;

private:
	CVKDevice* m_pDevice;
	CVKRenderTextureManager* m_pManager;
};
