#pragma once
#include "VKRenderer.h"


class CVKTexture2DManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTexture2D;


private:
	CVKTexture2DManager(CVKDevice* pDevice);
	virtual ~CVKTexture2DManager(void);


private:
	CVKTexture2D* Get(uint32_t name);
	CVKTexture2D* Create(uint32_t name);
	CVKTexture2D* Create(const char* szFileName, int baseLevel, int numLevels);
	void Destroy(CVKTexture2D* pTexture);


private:
	eastl::unordered_map<uint32_t, CVKTexture2D*> m_pTextures;

private:
	CVKDevice* m_pDevice;
};


class CVKTexture2DArrayManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTexture2DArray;


private:
	CVKTexture2DArrayManager(CVKDevice* pDevice);
	virtual ~CVKTexture2DArrayManager(void);


private:
	CVKTexture2DArray* Get(uint32_t name);
	CVKTexture2DArray* Create(uint32_t name);
	CVKTexture2DArray* Create(const char* szFileName, int baseLevel, int numLevels);
	void Destroy(CVKTexture2DArray* pTexture);


private:
	eastl::unordered_map<uint32_t, CVKTexture2DArray*> m_pTextures;

private:
	CVKDevice* m_pDevice;
};


class CVKTextureCubemapManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTextureCubemap;


private:
	CVKTextureCubemapManager(CVKDevice* pDevice);
	virtual ~CVKTextureCubemapManager(void);


private:
	CVKTextureCubemap* Get(uint32_t name);
	CVKTextureCubemap* Create(uint32_t name);
	CVKTextureCubemap* Create(const char* szFileName, int baseLevel, int numLevels);
	void Destroy(CVKTextureCubemap* pTexture);


private:
	eastl::unordered_map<uint32_t, CVKTextureCubemap*> m_pTextures;

private:
	CVKDevice* m_pDevice;
};
