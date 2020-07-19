#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DArray : public CGfxTexture2DArray
{
	friend class CGLES3Texture2DArrayManager;


private:
	CGLES3Texture2DArray(CGLES3Texture2DArrayManager* pManager, uint32_t name);
	virtual ~CGLES3Texture2DArray(void);
	virtual void Release(void);


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

public:
	void Bind(uint32_t unit) const;
	void BindImageTexture(uint32_t unit, int level, int layer) const;


private:
	CGLES3Texture* m_pTexture;

private:
	CGLES3Texture2DArrayManager* m_pManager;
};
