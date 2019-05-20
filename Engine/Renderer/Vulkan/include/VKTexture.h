#pragma once
#include "VKRenderer.h"


class CVKTexture : public CGfxResource
{
	friend class CVKTexture2D;
	friend class CVKTexture2DArray;
	friend class CVKTextureCubemap;
	friend class CVKRenderTexture;
	friend class CVKTransferBuffer;


private:
	CVKTexture(CVKDevice* pDevice);
	virtual ~CVKTexture(void);
	virtual void Release(void);


private:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageLayout GetImageLayout(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

private:
	bool Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageView vkImageView);
	bool Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageAspectFlags imageAspectFlags, VkImageUsageFlags imageUsageFlags, VkImageTiling imageTiling);
	void Destroy(void);

private:
	bool Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

private:
	bool PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout);
	bool PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags);


private:
	bool m_bExtern;

	VkImage m_vkImage;
	VkImageView m_vkImageView;
	VkImageLayout m_vkImageLayout;
	VkImageAspectFlags m_vkImageAspectFlags;

private:
	GfxTextureType m_type;
	GfxPixelFormat m_format;

	int m_width;
	int m_height;
	int m_layers;
	int m_levels;
	int m_samples;

private:
	CVKMemory* m_pMemory;

private:
	CVKDevice* m_pDevice;
};
