#pragma once
#include "utils.h"
#include "define.h"


class CComponent
{
	friend class CSceneNode;


public:
	CComponent(uint32_t name)
		: m_name(name)
		, m_pParentNode(NULL)
	{

	}
	CComponent(const CComponent &component)
		: m_name(component.m_name)
		, m_pParentNode(component.m_pParentNode)
	{

	}
	virtual ~CComponent(void)
	{

	}


public:
	virtual void TaskUpdate(float gameTime, float deltaTime) = 0;
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue) = 0;


protected:
	uint32_t m_name;
	CSceneNode *m_pParentNode;
};
