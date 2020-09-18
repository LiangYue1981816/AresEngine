#pragma once
#include "VKRenderer.h"


class CVKRenderTexture : public CGfxRenderTexture
{
public:
	CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name, HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples = 1);
	CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);
	virtual ~CVKRenderTexture(void);
	virtual void Release(void);


public:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetSamples(void) const;


private:
	CVKTexture* m_pTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKRenderTextureManager* m_pManager;
};
