#pragma once
#include "VKRenderer.h"


class CVKTextureManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDevice;
	friend class CVKTexture;
	friend class CVKTexture2D;
	friend class CVKTexture2DArray;
	friend class CVKTextureCubeMap;


private:
	CVKTextureManager(CVKDevice *pDevice);
	virtual ~CVKTextureManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHaveTexture2D(uint32_t name);
	bool IsHaveTexture2DArray(uint32_t name);
	bool IsHaveTextureCubeMap(uint32_t name);
	CVKTexture2D* CreateTexture2D(uint32_t name);
	CVKTexture2D* CreateTexture2D(const char *szFileName);
	CVKTexture2DArray* CreateTexture2DArray(uint32_t name);
	CVKTexture2DArray* CreateTexture2DArray(const char *szFileName);
	CVKTextureCubeMap* CreateTextureCubeMap(uint32_t name);
	CVKTextureCubeMap* CreateTextureCubeMap(const char *szFileName);
	void DestroyTexture2D(CGfxTexture2D *pTexture);
	void DestroyTexture2DArray(CGfxTexture2DArray *pTexture);
	void DestroyTextureCubeMap(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTexture2Ds;
	eastl::unordered_map<uint32_t, CGfxTexture2DArray*> m_pTexture2DArrays;
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextureCubeMaps;

private:
	CVKDevice *m_pDevice;
};
