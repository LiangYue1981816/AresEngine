#include "GLES3Renderer.h"


CGLES3Texture2D::CGLES3Texture2D(CGLES3Texture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)
{
	m_ptrTexture = CGLES3TexturePtr(new CGLES3Texture);
}

CGLES3Texture2D::~CGLES3Texture2D(void)
{
	Destroy();
}

void CGLES3Texture2D::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3Texture2D::GetTexture(void) const
{
	return (HANDLE)m_ptrTexture->GetTexture();
}

GfxTextureType CGLES3Texture2D::GetType(void) const
{
	return m_type;
}

GfxPixelFormat CGLES3Texture2D::GetFormat(void) const
{
	return m_format;
}

int CGLES3Texture2D::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGLES3Texture2D::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGLES3Texture2D::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

int CGLES3Texture2D::GetSamples(void) const
{
	return m_ptrTexture->GetSamples();
}

bool CGLES3Texture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	Destroy();
	{
		do {
			samples = std::max(samples, 1);

			m_format = format;
			m_type = samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

			if (m_ptrTexture->Create(m_type, format, width, height, 1, levels, samples) == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CGLES3Texture2D::Destroy(void)
{
	for (const auto& itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	m_size.clear();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_ptrTexture->Destroy();
}

bool CGLES3Texture2D::TransferTexture2D(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture->TransferTexture2D(format, level, xoffset, yoffset, width, height, size, data)) {
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		m_size[level] = size;
		CGfxProfiler::IncTextureDataSize(m_size[level]);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Texture2D::TransferTexture2DCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture->TransferTexture2DCompressed(format, level, xoffset, yoffset, width, height, size, data)) {
		CGfxProfiler::DecTextureDataSize(m_size[level]);
		m_size[level] = size;
		CGfxProfiler::IncTextureDataSize(m_size[level]);
		return true;
	}
	else {
		return false;
	}
}

void CGLES3Texture2D::Bind(uint32_t unit) const
{
	m_ptrTexture->Bind(unit);
}
