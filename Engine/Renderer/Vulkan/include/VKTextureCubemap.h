#pragma once
#include "VKRenderer.h"


class CVKTextureCubemap : public CGfxTextureCubemap
{
	friend class CVKTextureCubemapManager;


private:
	CVKTextureCubemap(CVKDevice* pDevice, CVKTextureCubemapManager* pManager, uint32_t name);
	virtual ~CVKTextureCubemap(void);
	virtual void Release(void);


public:
	VkImageView GetImageView(void) const;
	VkImageLayout GetImageLayout(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

public:
	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels);
	void Destroy(void);

public:
	bool TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	CVKTexturePtr m_ptrTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKTextureCubemapManager* m_pManager;
};
