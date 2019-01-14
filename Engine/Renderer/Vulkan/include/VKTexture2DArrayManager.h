#pragma once
#include "VKRenderer.h"


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
