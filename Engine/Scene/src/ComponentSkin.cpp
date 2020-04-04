#include "EngineHeader.h"


CComponentSkin::CComponentSkin(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentSkin::CComponentSkin(const CComponentSkin& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentSkin::~CComponentSkin(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

bool CComponentSkin::TaskUpdate(float gameTime, float deltaTime)
{
	return true;
}

bool CComponentSkin::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	return true;
}
