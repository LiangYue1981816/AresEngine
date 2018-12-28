#pragma once
#include "VKRenderer.h"


class CVKMeshDrawManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKMeshDraw;


private:
	CVKMeshDrawManager(CVKDevice *pDevice);
	virtual ~CVKMeshDrawManager(void);


private:
	CVKDevice* GetDevice(void) const;

private:
	bool IsHave(uint32_t name);
	CGfxMeshDraw* Create(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding);
	void Destroy(CGfxMeshDraw *pMeshDraw);


private:
	eastl::unordered_map<uint32_t, CGfxMeshDraw*> m_pMeshDraws;

private:
	CVKDevice *m_pDevice;
};
