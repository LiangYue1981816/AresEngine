#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DArray : public CGfxTexture2DArray
{
	friend class CGLES3Texture2DArrayManager;


private:
	CGLES3Texture2DArray(CGLES3Texture2DArrayManager *pManager, uint32_t name);
	virtual ~CGLES3Texture2DArray(void);
	virtual void Release(void);


public:
	HANDLE GetTexture(void) const;
	GfxTextureType GetType(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;

public:
	bool Create(GfxPixelFormat format, int width, int height, int levels, int layers);
	void Destroy(void);

public:
	bool TransferTexture2D(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data);
	bool TransferTexture2DCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data);

public:
	void Bind(uint32_t unit) const;


private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

	CGLES3TexturePtr m_ptrTexture;

private:
	CGLES3Texture2DArrayManager *m_pManager;

private:
	eastl::unordered_map<int, eastl::unordered_map<int, uint32_t>> m_size;
};
