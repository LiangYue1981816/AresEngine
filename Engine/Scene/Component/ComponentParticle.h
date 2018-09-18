#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentParticle : public CComponent
{
public:
	CComponentParticle(uint32_t name);
	virtual ~CComponentParticle(void);

public:
	virtual void TaskUpdate(float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread);
};

typedef CComponentPtr<CComponentParticle> CComponentParticlePtr;
