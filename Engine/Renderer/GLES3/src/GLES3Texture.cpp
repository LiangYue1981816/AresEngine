#include "GLES3Renderer.h"


CGLES3Texture::CGLES3Texture(CGLES3TextureManager *pManager)
	: m_pManager(pManager)

	, m_bExtern(false)
	, m_target(0)
	, m_texture(0)
{

}

CGLES3Texture::~CGLES3Texture(void)
{

}

bool CGLES3Texture::Create(uint32_t target, uint32_t texture)
{
	Destroy();

	m_bExtern = true;
	m_target = target;
	m_texture = texture;

	return true;
}

void CGLES3Texture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_bExtern = false;
	m_target = 0;
	m_texture = 0;
}

void CGLES3Texture::Bind(uint32_t unit)
{
	GLBindTexture(unit, m_target, m_texture);
}

uint32_t CGLES3Texture::GetTexture(void) const
{
	return m_texture;
}
