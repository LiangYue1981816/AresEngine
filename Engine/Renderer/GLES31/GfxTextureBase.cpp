#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxTextureBase.h"


CGfxTextureBase::CGfxTextureBase(uint32_t name)
	: m_name(name)

	, m_texture(0)
	, m_bExtern(GL_FALSE)

	, m_format(GL_INVALID_ENUM)
	, m_internalFormat(GL_INVALID_ENUM)

	, m_width(0)
	, m_height(0)

	, m_samples(0)
	, m_mipLevels(0)
	, m_arrayLayers(0)
{

}

CGfxTextureBase::~CGfxTextureBase(void)
{
	Destroy();
}

void CGfxTextureBase::Release(void)
{
	Renderer()->DestroyTexture(this);
}

uint32_t CGfxTextureBase::GetName(void) const
{
	return m_name;
}

bool CGfxTextureBase::CreateExtern(uint32_t texture)
{
	Destroy();

	m_texture = texture;
	m_bExtern = GL_TRUE;

	return true;
}

void CGfxTextureBase::Destroy(void)
{
	if (m_bExtern == GL_FALSE) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_texture = 0;
	m_bExtern = GL_FALSE;

	m_format = GL_INVALID_ENUM;
	m_internalFormat = GL_INVALID_ENUM;

	m_width = 0;
	m_height = 0;

	m_samples = 0;
	m_mipLevels = 0;
	m_arrayLayers = 0;
}

uint32_t CGfxTextureBase::GetTexture(void) const
{
	return m_texture;
}

uint32_t CGfxTextureBase::GetFormat(void) const
{
	return m_format;
}

uint32_t CGfxTextureBase::GetInternalFormat(void) const
{
	return m_internalFormat;
}

int CGfxTextureBase::GetWidth(void) const
{
	return m_width;
}

int CGfxTextureBase::GetHeight(void) const
{
	return m_height;
}

int CGfxTextureBase::GetLevels(void) const
{
	return m_mipLevels;
}

int CGfxTextureBase::GetLayers(void) const
{
	return m_arrayLayers;
}
