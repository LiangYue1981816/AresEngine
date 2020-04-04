#pragma once
#include "PreHeader.h"


class CALL_API CComponentSkin : public CComponent
{
public:
	CComponentSkin(uint32_t name);
	CComponentSkin(const CComponentSkin& component);
	virtual ~CComponentSkin(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);


private:
	int m_indexInstance;
};

typedef CComponentPtr<CComponentSkin> CComponentSkinPtr;
