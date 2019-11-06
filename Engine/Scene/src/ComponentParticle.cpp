#include "EngineHeader.h"


CComponentParticle::CComponentParticle(uint32_t name)
	: CComponent(name)
{

}

CComponentParticle::CComponentParticle(const CComponentParticle& component)
	: CComponent(component)
{

}

CComponentParticle::~CComponentParticle(void)
{

}

void CComponentParticle::TaskUpdate(float gameTime, float deltaTime)
{

}

void CComponentParticle::TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{

}
