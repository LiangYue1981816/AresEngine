#include "GfxHeader.h"


CGfxRenderer* CGfxRenderer::pInstance = nullptr;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

void CGfxRenderer::Create(GfxApi api, void *hDC, int width, int height, uint32_t format)
{
	if (pInstance == nullptr) {
		switch (api) {
		case GFX_API_GLES3:
			pInstance = new CGLES3Renderer(hDC, width, height, format);
			break;

		case GFX_API_VULKAN:
			break;

		case GFX_API_METAL:
			break;
		}
	}
}

void CGfxRenderer::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
		pInstance = nullptr;
	}
}

CGfxRenderer::CGfxRenderer(void *hDC, int width, int height, uint32_t format)
{
	pInstance = this;

#ifdef PLATFORM_WINDOWS
	gl3wInit();
#endif
}

CGfxRenderer::~CGfxRenderer(void)
{

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
