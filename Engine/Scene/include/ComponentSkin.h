#pragma once
#include "PreHeader.h"


class CComponentSkin : public CComponentMesh
{
public:
	CComponentSkin(uint32_t name);
	CComponentSkin(const CComponentSkin& component);
	virtual ~CComponentSkin(void);

public:
	virtual bool TaskUpdate(float gameTime, float deltaTime);
	virtual bool TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);
};

typedef CComponentPtr<CComponentSkin> CComponentSkinPtr;
