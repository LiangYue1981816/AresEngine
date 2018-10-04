#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTextureCubeMap : public CGfxTextureBase
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


private:
	CGfxTextureCubeMap(uint32_t name);
	virtual ~CGfxTextureCubeMap(void);


public:
	bool Load(const char *szFileName);
	bool LoadFace(const char *szFileName, GLsizei face);
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels);
	void Destroy(void);

public:
	bool TransferTextureCubeMap(const gli::texture_cube *texture);
	bool TransferTexture2D(GLsizei face, const gli::texture2d *texture);
	bool TransferTexture2D(GLsizei face, GLsizei level, GLenum format, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data);
	bool TransferTexture2DCompressed(GLsizei face, GLsizei level, GLenum format, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);


private:
	eastl::unordered_map<GLsizei, eastl::unordered_map<GLsizei, GLsizeiptr>> m_size;
};
