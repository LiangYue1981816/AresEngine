#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2D : public CGfxTexture2D
{
	friend class CGLES3Texture2DManager;


private:
	CGLES3Texture2D(CGLES3Texture2DManager* pManager, uint32_t name);
	virtual ~CGLES3Texture2D(void);
	virtual void Release(void);


public:
	HANDLE GetTexture(void) const;
	GfxTextureType GetType(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

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

public:
	void Bind(uint32_t unit) const;


private:
	GfxPixelFormat m_format;
	GfxTextureType m_type;

private:
	CGLES3TexturePtr m_ptrTexture;

private:
	CGLES3Texture2DManager* m_pManager;

private:
	eastl::unordered_map<int, uint32_t> m_size;
};
