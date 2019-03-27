#include "GLES3Renderer.h"


CGLES3RenderTexture::CGLES3RenderTexture(CGLES3RenderTextureManager *pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3RenderTexture::~CGLES3RenderTexture(void)
{
	Destroy();
}

void CGLES3RenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3RenderTexture::GetTexture(void) const
{
	return (HANDLE)m_ptrTexture->GetTexture();
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
	return m_ptrTexture->GetWidth();
}

int CGLES3RenderTexture::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3RenderTexture::GetSamples(void) const
{
	return m_ptrTexture->GetSamples();
}

bool CGLES3RenderTexture::Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples)
{
	Destroy();
	{
		do {
			samples = std::max(samples, 1);

			m_format = format;
			m_type = samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

			if (m_ptrTexture->Create(samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, (uint32_t)hExternalTexture, width, height, 1, 1, samples) == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CGLES3RenderTexture::Create(GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	Destroy();
	{
		do {
			samples = std::max(samples, 1);

			m_format = format;
			m_type = samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

			if (m_ptrTexture->Create(samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples) == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CGLES3RenderTexture::Destroy(void)
{
	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_ptrTexture->Destroy();
}

void CGLES3RenderTexture::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
