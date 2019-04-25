#pragma once
#include "VKRenderer.h"


class CVKTexture : public CGfxResource
{
	friend class CVKRenderTexture;
	friend class CVKTexture2D;
	friend class CVKTexture2DArray;
	friend class CVKTextureCubeMap;


private:
	CVKTexture(CVKDevice* pDevice);
	virtual ~CVKTexture(void);
	virtual void Release(void);


private:
	VkImageView GetImageView(void) const;

	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

private:
	bool Create(GfxTextureType type, VkImageView vkImageView, int width, int height, int layers, int levels, int samples);
	bool Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkImageAspectFlags imageAspectFlags);
	void Destroy(void);

private:
	bool TransferTexture2D(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTexture2DArray(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTexture2DArrayCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTextureCubeMap(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TransferTextureCubeMapCompressed(GfxPixelFormat format, GfxCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

private:
	bool MemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout layout);


private:
	bool m_bExtern;

	VkImage m_vkImage;
	VkImageView m_vkImageView;
	VkImageLayout m_vkImageLayout;
	VkImageAspectFlags m_vkImageAspectFlags;

	CVKMemory* m_pMemory;

private:
	GfxTextureType m_type;
	GfxPixelFormat m_format;

	int m_width;
	int m_height;
	int m_layers;
	int m_levels;
	int m_samples;

private:
	eastl::map<int, VkBufferImageCopy> m_transferRegions;
	eastl::map<int, eastl::vector<uint8_t>> m_transferBuffers;

private:
	CVKDevice* m_pDevice;
};

typedef CGfxResourcePtr<CVKTexture> CVKTexturePtr;
