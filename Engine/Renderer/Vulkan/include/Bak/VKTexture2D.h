#pragma once
#include "VKRenderer.h"


class CVKTexture2D : public CVKTexture, public CGfxTexture2D
{
	friend class CVKTexture2DManager;


private:
	CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name);
	virtual ~CVKTexture2D(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetTexture(void) const;

public:
	GfxPixelFormat GetFormat(void) const;
	GfxTextureType GetType(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels, int samples = 1);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void* data);
	bool TransferTexture2DCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_levels;
	int m_samples;

private:
	eastl::unordered_map<int, uint32_t> m_size;

private:
	CVKTexture2DManager* m_pManager;
};
