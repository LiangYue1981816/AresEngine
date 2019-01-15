#pragma once
#include "VKRenderer.h"


class CVKTexture2DManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTexture2D;


private:
	CVKTexture2DManager(CVKDevice *pDevice);
	virtual ~CVKTexture2DManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHave(uint32_t name);
	CVKTexture2D* Create(uint32_t name);
	CVKTexture2D* Create(const char *szFileName);
	void Destroy(CGfxTexture2D *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2D*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};


class CVKTexture2DArrayManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTexture2DArray;


private:
	CVKTexture2DArrayManager(CVKDevice *pDevice);
	virtual ~CVKTexture2DArrayManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHave(uint32_t name);
	CVKTexture2DArray* Create(uint32_t name);
	CVKTexture2DArray* Create(const char *szFileName);
	void Destroy(CGfxTexture2DArray *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTexture2DArray*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};


class CVKTextureCubeMapManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKTextureCubeMap;


private:
	CVKTextureCubeMapManager(CVKDevice *pDevice);
	virtual ~CVKTextureCubeMapManager(void);


private:
	CVKDevice* GetDevice(void) const;


private:
	bool IsHave(uint32_t name);
	CVKTextureCubeMap* Create(uint32_t name);
	CVKTextureCubeMap* Create(const char *szFileName);
	void Destroy(CGfxTextureCubeMap *pTexture);


private:
	eastl::unordered_map<uint32_t, CGfxTextureCubeMap*> m_pTextures;

private:
	CVKDevice *m_pDevice;
};
