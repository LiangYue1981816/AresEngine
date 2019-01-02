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

bool CGLES3Texture::Create(uint32_t target, GfxPixelFormat pixelFormat, int width, int height, int levels, int layers, int samples)
{
	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	switch (target) {
	case GL_TEXTURE_2D:
		m_target = target;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, levels, glFormat.Internal, width, height);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;

	case GL_TEXTURE_2D_MULTISAMPLE:
		m_target = target;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, glFormat.Internal, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		break;

	case GL_TEXTURE_2D_ARRAY:
		m_target = target;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, glFormat.Internal, width, height, layers);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		break;

	case GL_TEXTURE_CUBE_MAP:
		m_target = target;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, glFormat.Internal, width, height);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		break;

	default:
		return false;
	}

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
