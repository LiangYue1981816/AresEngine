#pragma once
#include "VKRenderer.h"


class CVKTextureCubemap : public CGfxTextureCubemap
{
public:
	CVKTextureCubemap(CVKDevice* pDevice, CVKTextureCubemapManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int levels);
	virtual ~CVKTextureCubemap(void);
	virtual void Release(void);


public:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;


private:
	CVKTexture* m_pTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKTextureCubemapManager* m_pManager;
};
