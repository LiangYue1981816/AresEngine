#include "GLES3Renderer.h"


CGLES3TextureBase::CGLES3TextureBase(CGLES3TextureManager *pManager)
	: m_pManager(pManager)

	, m_bExtern(false)

	, m_target(GL_INVALID_ENUM)
	, m_texture(0)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
{

}

CGLES3TextureBase::~CGLES3TextureBase(void)
{

}

bool CGLES3TextureBase::Create(uint32_t target, uint32_t texture)
{
	Destroy();

	m_bExtern = true;

	m_target = target;
	m_texture = texture;

	return true;
}

void CGLES3TextureBase::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_bExtern = false;

	m_target = GL_INVALID_ENUM;
	m_texture = 0;

	m_format = GFX_PIXELFORMAT_UNDEFINED;
}

void CGLES3TextureBase::Bind(uint32_t unit)
{
	GLBindTexture(unit, m_target, m_texture);
}

bool CGLES3TextureBase::IsExtern(void) const
{
	return m_bExtern;
}

uint32_t CGLES3TextureBase::GetTarget(void) const
{
	return m_target;
}

uint32_t CGLES3TextureBase::GetTexture(void) const
{
	return m_texture;
}

GfxPixelFormat CGLES3TextureBase::GetFormat(void) const
{
	return m_format;
}
