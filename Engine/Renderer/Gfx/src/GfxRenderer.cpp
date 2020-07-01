#include "GfxHeader.h"


CGfxRenderer* CGfxRenderer::pInstance = nullptr;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

CGfxRenderer::CGfxRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
{
	pInstance = this;
}

CGfxRenderer::~CGfxRenderer(void)
{
	pInstance = nullptr;
}

void CGfxRenderer::SetShaderCachePath(const char* szPath) const
{
#ifdef PLATFORM_WINDOWS
	ShaderCompiler()->SetCachePath(szPath);
#endif
}

void CGfxRenderer::SetShaderIncludePath(const char* szPath) const
{
#ifdef PLATFORM_WINDOWS
	ShaderCompiler()->AddIncludePath(szPath);
#endif
}
