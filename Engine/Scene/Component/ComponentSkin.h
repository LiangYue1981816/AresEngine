#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentSkin : public CComponent
{
public:
	CComponentSkin(uint32_t name);
	virtual ~CComponentSkin(void);

public:
	virtual void Update(float deltaTime);
	virtual void UpdateCamera(CGfxCamera *pCamera, int indexThread);
};

typedef CComponentPtr<CComponentSkin> CComponentSkinPtr;
