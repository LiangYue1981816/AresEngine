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
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels);
	void Destroy(void);

public:
	bool TransferTexture2D(const gli::texture2d *texture);
	bool TransferTexture2D(GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data);
	bool TransferTexture2DCompressed(GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);


private:
	eastl::unordered_map<GLsizei, GLsizeiptr> m_size;
};
