#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTextureCubeMap : public CGfxTextureBase
{
	friend class CGfxMaterial;
	friend class CGfxTextureManager;


private:
	CGfxTextureCubeMap(GLuint name);
	virtual ~CGfxTextureCubeMap(void);


public:
	bool Load(const char *szFileName);
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels);

public:
	bool TransferTextureCubeMap(const gli::texture_cube *texture);
	bool TransferTexture2D(GLsizei face, const gli::texture2d *texture);
	bool TransferTexture2D(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, const GLvoid *data);
	bool TransferTexture2DCompressed(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);
};
