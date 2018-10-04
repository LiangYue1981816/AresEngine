#pragma once
#include "GfxRenderer.h"


class CGfxTextureBase : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxTextureManager;


protected:
	CGfxTextureBase(uint32_t name);
	virtual ~CGfxTextureBase(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;


public:
	virtual bool Load(const char *szFileName) = 0;
	virtual bool CreateExtern(GLuint texture);
	virtual void Destroy(void);

public:
	GLuint GetTexture(void) const;

	GLenum GetFormat(void) const;
	GLenum GetInternalFormat(void) const;

	GLsizei GetWidth(void) const;
	GLsizei GetHeight(void) const;

	GLsizei GetLevels(void) const;
	GLsizei GetLayers(void) const;


protected:
	uint32_t m_name;

protected:
	GLuint m_texture;
	GLboolean m_extern;

protected:
	GLenum m_format;
	GLenum m_internalFormat;

	GLsizei m_width;
	GLsizei m_height;

	GLsizei m_mipLevels;
	GLsizei m_arrayLayers;
};
