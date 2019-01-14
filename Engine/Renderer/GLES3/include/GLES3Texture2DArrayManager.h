#pragma once
#include "GLES3Renderer.h"


class CGLES3Texture2DArrayManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Texture2DArray;


private:
	CGLES3Texture2DArrayManager(void);
	virtual ~CGLES3Texture2DArrayManager(void);


private:
	bool IsHave(uint32_t name);
	CGLES3Texture2DArray* Create(uint32_t name);
	CGLES3Texture2DArray* Create(const char *szFileName);
	void Destroy(CGfxTexture2DArray *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2DArray*> m_pTextures;
};
