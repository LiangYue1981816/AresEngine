#include "GfxHeader.h"


CGLES3TextureBase::CGLES3TextureBase(CGLES3TextureManager *pManager, uint32_t name)
	: CGfxTextureBase(name)
	, m_pManager(pManager)

	, m_texture(0)
	, m_bExtern(GL_FALSE)

	, m_format(GL_INVALID_ENUM)
	, m_internalFormat(GL_INVALID_ENUM)

	, m_levels(0)
{

}

CGLES3TextureBase::~CGLES3TextureBase(void)
{

}

bool CGLES3TextureBase::Create(uint32_t texture)
{
	Destroy();

	m_texture = texture;
	m_bExtern = GL_TRUE;

	return true;
}

void CGLES3TextureBase::Destroy(void)
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

	m_levels = 0;
}

void CGLES3TextureBase::Bind(uint32_t unit)
{
	GLBindTexture(unit, GetTarget(), m_texture);
}

uint32_t CGLES3TextureBase::GetTexture(void) const
{
	return m_texture;
}

uint32_t CGLES3TextureBase::GetFormat(void) const
{
	return m_format;
}

uint32_t CGLES3TextureBase::GetInternalFormat(void) const
{
	return m_internalFormat;
}
