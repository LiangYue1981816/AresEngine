#pragma once
#include "Component.h"


class CALL_API CComponentSkin : public CComponent
{
public:
	CComponentSkin(uint32_t name);
	CComponentSkin(const CComponentSkin& component);
	virtual ~CComponentSkin(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread);
};

typedef CComponentPtr<CComponentSkin> CComponentSkinPtr;
