#include "GfxHeader.h"

#ifdef PLATFORM_WINDOWS
#  include <gl3w.h>
#endif


CGfxRenderer* CGfxRenderer::pInstance = nullptr;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

CGfxRenderer::CGfxRenderer(void *hDC, int width, int height, GfxPixelFormat pixelFormat)
{
	pInstance = this;

#ifdef PLATFORM_WINDOWS
	gl3wInit();
#endif
}

CGfxRenderer::~CGfxRenderer(void)
{
	pInstance = nullptr;
}

void CGfxRenderer::SetShaderCachePath(const char *szPath) const
{
#ifdef PLATFORM_WINDOWS
	ShaderCompiler()->SetCachePath(szPath);
#endif
}

void CGfxRenderer::SetShaderIncludePath(const char *szPath) const
{
#ifdef PLATFORM_WINDOWS
	ShaderCompiler()->AddIncludePath(szPath);
#endif
}
