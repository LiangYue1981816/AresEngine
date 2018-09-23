#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentParticle : public CComponent
{
public:
	CComponentParticle(uint32_t name);
	CComponentParticle(const CComponentParticle &component);
	virtual ~CComponentParticle(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread);
};

typedef CComponentPtr<CComponentParticle> CComponentParticlePtr;
