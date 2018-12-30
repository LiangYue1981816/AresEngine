#pragma once
#include "GLES3Renderer.h"


class CGLES3RenderTextureManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3RenderTexture;


private:
	CGLES3RenderTextureManager(void);
	virtual ~CGLES3RenderTextureManager(void);


private:
	bool IsHaveRenderTexture(uint32_t name);
	CGLES3RenderTexture* CreateRenderTexture(uint32_t name);
	void Destroy(CGfxRenderTexture *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxRenderTexture*> m_pTextures;
};
