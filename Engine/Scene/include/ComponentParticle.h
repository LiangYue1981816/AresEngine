#pragma once
#include "PreHeader.h"


class CComponentParticle : public CComponent
{
public:
	CComponentParticle(uint32_t name);
	CComponentParticle(const CComponentParticle& component);
	virtual ~CComponentParticle(void);

public:
	virtual bool TaskUpdate(float gameTime, float deltaTime);
	virtual bool TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);


private:
	int m_indexInstance;
};

typedef CComponentPtr<CComponentParticle> CComponentParticlePtr;
