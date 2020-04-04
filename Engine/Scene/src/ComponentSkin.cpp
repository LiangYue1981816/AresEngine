#include "EngineHeader.h"


CComponentSkin::CComponentSkin(uint32_t name)
	: CComponentMesh(name)
{

}

CComponentSkin::CComponentSkin(const CComponentSkin& component)
	: CComponentMesh(component)
{

}

CComponentSkin::~CComponentSkin(void)
{

}

bool CComponentSkin::TaskUpdate(float gameTime, float deltaTime)
{
	CComponentMesh::TaskUpdate(gameTime, deltaTime);
	return true;
}

bool CComponentSkin::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	return CComponentMesh::TaskUpdateCamera(pCamera, pRenderQueue, mask, bComputeLOD, indexThread);
}
