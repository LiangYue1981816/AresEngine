#pragma once
#include "PreHeader.h"


class CALL_API CComponentParticle : public CComponent
{
public:
	CComponentParticle(uint32_t name);
	CComponentParticle(const CComponentParticle& component);
	virtual ~CComponentParticle(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);


private:
	int m_indexInstance;
};

typedef CComponentPtr<CComponentParticle> CComponentParticlePtr;
