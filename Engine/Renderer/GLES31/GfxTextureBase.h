#pragma once
#include "GfxRenderer.h"


class CGfxTextureBase : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxMaterial;
	friend class CGfxTextureManager;


protected:
	CGfxTextureBase(GLuint name);
	virtual ~CGfxTextureBase(void);
	virtual void Release(void);


public:
	virtual bool CreateExtern(GLuint texture);
	virtual void Destroy(void);

public:
	bool IsValid(void) const;

	GLuint GetName(void) const;
	GLuint GetTexture(void) const;

	GLenum GetFormat(void) const;
	GLenum GetInternalFormat(void) const;

	GLuint GetWidth(void) const;
	GLuint GetHeight(void) const;


protected:
	GLuint m_name;

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
