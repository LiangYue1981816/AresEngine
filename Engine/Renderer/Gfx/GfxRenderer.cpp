#include "GfxRenderer.h"


CGfxRenderer* CGfxRenderer::pInstance = nullptr;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

CGfxRenderer::CGfxRenderer(void *hDC, const char *szShaderCachePath, int width, int height, uint32_t format)
{
	pInstance = this;
}

CGfxRenderer::~CGfxRenderer(void)
{
	pInstance = nullptr;
}
