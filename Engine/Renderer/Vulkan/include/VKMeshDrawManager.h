#pragma once
#include "VKRenderer.h"


class CVKMeshDrawManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKMeshDraw;


private:
	CVKMeshDrawManager(CVKDevice* pDevice);
	virtual ~CVKMeshDrawManager(void);


private:
	CVKMeshDraw* Get(uint32_t name);
	CVKMeshDraw* Create(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding);
	void Destroy(CVKMeshDraw* pMeshDraw);


private:
	eastl::unordered_map<uint32_t, CVKMeshDraw*> m_pMeshDraws;

private:
	CVKDevice* m_pDevice;
};
