#pragma once
#include "VKRenderer.h"


class CVKTexture2DArray : public CGfxTexture2DArray
{
public:
	CVKTexture2DArray(CVKDevice* pDevice, CVKTexture2DArrayManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int layers, int levels);
	virtual ~CVKTexture2DArray(void);
	virtual void Release(void);


public:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;


private:
	CVKTexture* m_pTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKTexture2DArrayManager* m_pManager;
};
