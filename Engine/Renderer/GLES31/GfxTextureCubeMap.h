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
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels);
	void Destroy(void);

public:
	bool TransferTextureCubeMap(const gli::texture_cube *texture);
	bool TransferTexture2D(GLsizei face, const gli::texture2d *texture);
	bool TransferTexture2D(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data);
	bool TransferTexture2DCompressed(GLsizei face, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);


private:
	eastl::unordered_map<GLsizei, eastl::unordered_map<GLsizei, GLsizeiptr>> m_size;
};
