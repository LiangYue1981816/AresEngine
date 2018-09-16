#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentParticle : public CComponent
{
public:
	CComponentParticle(uint32_t name);
	virtual ~CComponentParticle(void);

public:
	virtual void Update(float deltaTime);
	virtual void UpdateCamera(CGfxCamera *pCamera, int indexThread);
};

typedef CComponentPtr<CComponentParticle> CComponentParticlePtr;
