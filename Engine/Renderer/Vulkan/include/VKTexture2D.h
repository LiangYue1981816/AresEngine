#pragma once
#include "VKRenderer.h"


class CVKTexture2D : public CGfxTexture2D
{
	friend class CVKTexture2DManager;


private:
	CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name);
	virtual ~CVKTexture2D(void);
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
	int GetSamples(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels, int samples = 1);
	void Destroy(void);

public:
	bool Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	CVKTexturePtr m_ptrTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKTexture2DManager* m_pManager;
};
