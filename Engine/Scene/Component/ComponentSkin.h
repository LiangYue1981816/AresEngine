#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentSkin : public CComponent
{
public:
	CComponentSkin(uint32_t name);
	CComponentSkin(const CComponentSkin &component);
	virtual ~CComponentSkin(void);

public:
	virtual void TaskUpdate(float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread);
};

typedef CComponentPtr<CComponentSkin> CComponentSkinPtr;
