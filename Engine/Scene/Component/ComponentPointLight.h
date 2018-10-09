#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentPointLight : public CComponent
{
public:
	CComponentPointLight(uint32_t name);
	CComponentPointLight(const CComponentPointLight &component);
	virtual ~CComponentPointLight(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
