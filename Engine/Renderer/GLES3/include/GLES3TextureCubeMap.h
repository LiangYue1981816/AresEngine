#pragma once
#include "GLES3Renderer.h"


class CGLES3TextureCubeMap : public CGfxTextureCubeMap
{
	friend class CGLES3TextureCubeMapManager;


private:
	CGLES3TextureCubeMap(CGLES3TextureCubeMapManager *pManager, uint32_t name);
	virtual ~CGLES3TextureCubeMap(void);
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

public:
	void Bind(uint32_t unit) const;


private:
	uint32_t m_name;

private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	CGLES3TexturePtr m_ptrTexture;

private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;

private:
	CGLES3TextureCubeMapManager *m_pManager;
};
