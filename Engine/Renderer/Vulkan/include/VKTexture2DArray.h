#pragma once
#include "VKRenderer.h"


class CVKTexture2DArray : public CGfxTexture2DArray
{
	friend class CVKTexture2DArrayManager;


private:
	CVKTexture2DArray(CVKDevice* pDevice, uint32_t name);
	virtual ~CVKTexture2DArray(void);
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
	int GetLayers(void) const;
	int GetLevels(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int layers, int levels);
	void Destroy(void);

public:
	bool Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	CVKTexturePtr m_ptrTexture;

private:
	CVKDevice* m_pDevice;
};
