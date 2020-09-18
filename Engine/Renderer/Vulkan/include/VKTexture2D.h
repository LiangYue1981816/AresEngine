#pragma once
#include "VKRenderer.h"


class CVKTexture2D : public CGfxTexture2D
{
public:
	CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int levels, int samples);
	virtual ~CVKTexture2D(void);
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
	int GetSamples(void) const;


private:
	CVKTexture* m_pTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKTexture2DManager* m_pManager;
};
