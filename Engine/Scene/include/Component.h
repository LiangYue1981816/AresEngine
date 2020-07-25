#pragma once
#include "PreHeader.h"


class CComponent : public CComponentBase
{
	friend class CSceneNode;


public:
	CComponent(uint32_t name)
		: m_name(name)
		, m_pParentNode(nullptr)
	{

	}
	CComponent(const CComponent& component)
		: CComponentBase(component)
		, m_name(component.m_name)
		, m_pParentNode(component.m_pParentNode)
	{

	}
	virtual ~CComponent(void)
	{

	}


public:
	virtual bool TaskUpdate(float gameTime, float deltaTime) = 0;
	virtual bool TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread) = 0;


protected:
	uint32_t m_name;
	CSceneNode* m_pParentNode;
};
