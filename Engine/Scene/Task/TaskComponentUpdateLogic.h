#pragma once
#include "Engine.h"
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T> class CTaskComponentUpdateLogic : public CTask
{
public:
	CTaskComponentUpdateLogic(void)
		: m_indexThread(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateLogic(void)
	{

	}


public:
	void SetParams(int indexThread, CComponentManager<T> *pComponentManager, float gameTime, float deltaTime)
	{
		m_gameTime = gameTime;
		m_deltaTime = deltaTime;

		m_indexThread = indexThread;
		m_pComponentManager = pComponentManager;
	}

	virtual void TaskFunc(void *pParams)
	{
		if (m_pComponentManager) {
			size_t count = (m_pComponentManager->GetComponentCount() + (THREAD_COUNT - m_pComponentManager->GetComponentCount() % THREAD_COUNT)) / THREAD_COUNT;
			size_t indexBegin = max(count * m_indexThread, 0);
			size_t indexEnd = min(count * (m_indexThread + 1), m_pComponentManager->GetComponentCount());

			for (size_t index = indexBegin; index < indexEnd; index++) {
				CComponent *pComponent = (CComponent *)m_pComponentManager->GetComponentByIndex(index);
				pComponent->TaskUpdate(m_gameTime, m_deltaTime);
			}
		}
	}


private:
	float m_gameTime;
	float m_deltaTime;

private:
	int m_indexThread;
	CComponentManager<T> *m_pComponentManager;
};
