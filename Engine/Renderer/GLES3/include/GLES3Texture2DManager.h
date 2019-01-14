#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2D;


private:
	CGLES3Texture2DManager(void);
	virtual ~CGLES3Texture2DManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3Texture2D* Create(uint32_t name);
	CGLES3Texture2D* Create(const char *szFileName);
	void Destroy(CGfxTexture2D *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTextures;
};
