#pragma once
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


class CGfxTexture2DArray : public CGfxTextureBase
{
	friend class CGfxMaterial;
	friend class CGfxTextureManager;


private:
	CGfxTexture2DArray(GLuint name);
	virtual ~CGfxTexture2DArray(void);
	virtual void Free(void);


public:
	bool Load(const char *szFileName);
	bool LoadLayer(const char *szFileName, GLsizei layer);
	bool Create(GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei mipLevels, GLsizei arrayLayers);

public:
	bool TransferTexture2DArray(const gli::texture2d_array *texture);
	bool TransferTexture2D(GLsizei layer, const gli::texture2d *texture);
	bool TransferTexture2D(GLsizei layer, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLenum type, GLsizei size, const GLvoid *data);
	bool TransferTexture2DCompressed(GLsizei layer, GLsizei level, GLenum format, GLsizei width, GLsizei height, GLsizei size, const GLvoid *data);


private:
	eastl::unordered_map<GLsizei, eastl::unordered_map<GLsizei, GLsizeiptr>> m_size;
};
