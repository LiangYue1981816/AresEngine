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
	CGLES3RenderTexture* Get(uint32_t name);
	CGLES3RenderTexture* Create(uint32_t name);
	void Destroy(CGLES3RenderTexture* pRenderTexture);


private:
	eastl::unordered_map<uint32_t, CGLES3RenderTexture*> m_pRenderTextures;
};
