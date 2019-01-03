#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice *pDevice, CVKRenderTextureManager *pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
{

}

CVKRenderTexture::~CVKRenderTexture(void)
{

}

void CVKRenderTexture::Release(void)
{

}

bool CVKRenderTexture::Create(GfxPixelFormat pixelFormat, int width, int height, int samples)
{
	return true;
}

void CVKRenderTexture::Destroy(void)
{

}
