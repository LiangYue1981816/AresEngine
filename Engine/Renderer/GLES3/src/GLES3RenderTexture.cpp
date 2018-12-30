#include "GLES3Renderer.h"


CGLES3RenderTexture::CGLES3RenderTexture(CGLES3RenderTextureManager *pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)
{

}

CGLES3RenderTexture::~CGLES3RenderTexture(void)
{
	Destroy();
}

void CGLES3RenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3RenderTexture::Create(GfxPixelFormat pixelFormat, int width, int height, int samples)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_samples = samples;

#if GLES_VER == 310
	if (m_samples == 0)
#endif
	{
		m_target = GL_TEXTURE_2D;

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, glFormat.Internal, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#if GLES_VER == 310
	else
	{
		m_target = GL_TEXTURE_2D_MULTISAMPLE;

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, glFormat.Internal, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
#endif

	return true;
}

void CGLES3RenderTexture::Destroy(void)
{
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
	}

	m_target = GL_INVALID_ENUM;
	m_texture = 0;

	m_width = 0;
	m_height = 0;
	m_samples = 0;
	m_format = GFX_PIXELFORMAT_UNDEFINED;
}

uint32_t CGLES3RenderTexture::GetTarget(void) const
{
	return m_target;
}

uint32_t CGLES3RenderTexture::GetTexture(void) const
{
	return m_texture;
}
