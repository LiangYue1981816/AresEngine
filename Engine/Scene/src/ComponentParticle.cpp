#include "EngineHeader.h"


CComponentParticle::CComponentParticle(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();
}

CComponentParticle::CComponentParticle(const CComponentParticle& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();
}

CComponentParticle::~CComponentParticle(void)
{
	RenderSystem()->GetGPUScene()->RemoveInstance(m_indexInstance);
}

void CComponentParticle::TaskUpdate(float gameTime, float deltaTime)
{

}

void CComponentParticle::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{

}
