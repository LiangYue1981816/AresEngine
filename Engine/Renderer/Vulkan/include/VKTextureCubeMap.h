#pragma once
#include "VKRenderer.h"


class CVKTextureCubeMap : public CVKTexture, public CGfxTextureCubeMap
{
	friend class CVKTextureCubeMapManager;


private:
	CVKTextureCubeMap(CVKDevice *pDevice, CVKTextureCubeMapManager *pManager, uint32_t name);
	virtual ~CVKTextureCubeMap(void);
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

public:
	bool Create(HANDLE hExternTexture);
	bool Create(GfxPixelFormat format, int width, int height, int levels);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(GfxPixelFormat format, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	int m_width;
	int m_height;
	int m_levels;

private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;

private:
	CVKTextureCubeMapManager *m_pManager;
};
