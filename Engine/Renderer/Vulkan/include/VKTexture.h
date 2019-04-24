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

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

private:
	bool CreateView(VkImageView vkImageView);
	bool CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format, int levels, int layers);
	bool CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int layers, int levels, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);
	void Destroy(void);

private:
	bool TransferTexture2D(int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data);
	bool TransferTexture2DCompressed(int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTexture2DArray(int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data);
	bool TransferTexture2DArrayCompressed(int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TransferTextureCubeMap(GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data);
	bool TransferTextureCubeMapCompressed(GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	bool m_bExtern;
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	CVKMemory* m_pMemory;

private:
	int m_width;
	int m_height;
	int m_layers;
	int m_levels;
	int m_samples;

private:
	CVKDevice* m_pDevice;
};

typedef CGfxResourcePtr<CVKTexture> CVKTexturePtr;
