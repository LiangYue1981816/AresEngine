#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture : public CGfxResource
{
	friend class CGLES3Texture2D;
	friend class CGLES3Texture2DArray;
	friend class CGLES3TextureCubemap;
	friend class CGLES3RenderTexture;


private:
	CGLES3Texture(void);
	virtual ~CGLES3Texture(void);
	virtual void Release(void) {}


private:
	uint32_t GetTarget(void) const;
	uint32_t GetTexture(void) const;

	GfxTextureType GetType(void) const;
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetLayers(void) const;
	int GetLevels(void) const;
	int GetSamples(void) const;

private:
	bool Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples, uint32_t texture);
	bool Create(GfxTextureType type, GfxPixelFormat format, int width, int height, int layers, int levels, int samples);
	void Destroy(void);

private:
	bool Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

	bool TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);
	bool TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data);

private:
	void Bind(uint32_t unit) const;
	void BindImageTexture(uint32_t unit, int level, int layer) const;


private:
	bool m_bExtern;

	uint32_t m_target;
	uint32_t m_texture;

private:
	GfxTextureType m_type;
	GfxPixelFormat m_format;

	int m_width;
	int m_height;
	int m_layers;
	int m_levels;
	int m_samples;
};
