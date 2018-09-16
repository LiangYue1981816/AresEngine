#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTexture2D : public CGfxTextureBase
{
	friend class CGfxMaterial;
	friend class CGfxTextureManager;


private:
	CGfxTexture2D(GLuint name);
	virtual ~CGfxTexture2D(void);


public:
	bool Load(const char *szFileName);
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLuint mipLevels);

public:
	bool TransferTexture2D(const gli::texture2d *texture);
	bool TransferTexture2D(GLuint level, GLenum format, GLsizei width, GLsizei height, GLenum type, const GLvoid *data);
	bool TransferTexture2DCompressed(GLuint level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);
};
