#include "GLES3Renderer.h"


CGLES3RenderTexture::CGLES3RenderTexture(CGLES3RenderTextureManager *pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_name(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_samples(0)

	, m_bExtern(false)
	, m_texture(0)
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

uint32_t CGLES3RenderTexture::GetName(void) const
{
	return m_name;
}

HANDLE CGLES3RenderTexture::GetTexture(void) const
{
	return (HANDLE)m_texture;
}

GfxPixelFormat CGLES3RenderTexture::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CGLES3RenderTexture::GetType(void) const
{
	return m_type;
}

int CGLES3RenderTexture::GetWidth(void) const
{
	return m_width;
}

int CGLES3RenderTexture::GetHeight(void) const
{
	return m_height;
}

int CGLES3RenderTexture::GetSamples(void) const
{
	return m_samples;
}

bool CGLES3RenderTexture::Create(HANDLE hExternTexture, GfxPixelFormat pixelFormat, int width, int height, int samples)
{
	Destroy();

	m_bExtern = true;
	m_texture = (uint32_t)hExternTexture;

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_samples = std::max(samples, 1);
	m_type = m_samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

	return true;
}

bool CGLES3RenderTexture::Create(GfxPixelFormat pixelFormat, int width, int height, int samples, bool bTransient)
{
	Destroy();

	gli::gl GL(gli::gl::PROFILE_ES30);
	gli::gl::format glFormat = GL.translate((gli::format)pixelFormat);

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_samples = std::max(samples, 1);
	m_type = m_samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

	if (m_samples == 1) {
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, glFormat.Internal, m_width, m_height);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, glFormat.Internal, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}

	return true;
}

void CGLES3RenderTexture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_bExtern = false;
	m_texture = 0;

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_samples = 0;
}

void CGLES3RenderTexture::Bind(uint32_t unit) const
{
	GLBindTexture(unit, GL_TEXTURE_2D, m_texture);
}
