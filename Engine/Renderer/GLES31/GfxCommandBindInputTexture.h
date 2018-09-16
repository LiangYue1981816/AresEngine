#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandBindInputTexture : public CGfxCommandBase
{
public:
	CGfxCommandBindInputTexture(const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode)
		: m_name(szName)
		, m_texture(texture)
		, m_minFilter(minFilter)
		, m_magFilter(magFilter)
		, m_addressMode(addressMode)
	{

	}
	virtual ~CGfxCommandBindInputTexture(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		Renderer()->BindInputTexture(m_name.c_str(), m_texture, m_minFilter, m_magFilter, m_addressMode);
	}


private:
	eastl::string m_name;
	GLuint m_texture;
	GLenum m_minFilter;
	GLenum m_magFilter;
	GLenum m_addressMode;
};
