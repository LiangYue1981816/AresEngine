#pragma once
#include "VKRenderer.h"


class CVKTexture : public CGfxResource
{
	friend class CVKTexture2D;
	friend class CVKTexture2DArray;
	friend class CVKTextureCubemap;
	friend class CVKRenderTexture;


private:
	CVKTexture(CVKDevice* pDevice, GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImage vkImage);
	CVKTexture(CVKDevice* pDevice, GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageUsageFlags imageUsageFlags, VkImageTiling imageTiling);
	virtual ~CVKTexture(void);
	virtual void Release(void) {}


private:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;
	VkDeviceSize GetMemorySize(void) const;

	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;


private:
	bool m_bExtern;

private:
	GfxTextureType m_type;
	GfxPixelFormat m_format;

	int m_width;
	int m_height;
	int m_layers;
	int m_levels;
	int m_samples;

private:
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	VkImageAspectFlags m_vkImageAspectFlags;

private:
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};
