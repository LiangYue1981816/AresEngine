#pragma once
#include "VKRenderer.h"


class CVKRenderTexture : public CGfxRenderTexture
{
	friend class CVKRenderTextureManager;


private:
	CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name);
	virtual ~CVKRenderTexture(void);
	virtual void Release(void);


public:
	VkImage GetImage(void) const;
	VkImageView GetImageView(void) const;
	VkImageAspectFlags GetImageAspectFlags(void) const;

public:
	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetSamples(void) const;

public:
	bool Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples = 1);
	bool Create(GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);
	void Destroy(void);


private:
	CVKTexture* m_pTexture;

private:
	CVKDevice* m_pDevice;

private:
	CVKRenderTextureManager* m_pManager;
};
