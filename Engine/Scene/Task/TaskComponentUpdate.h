#pragma once
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T> class CTaskComponentUpdate : public CTask
{
public:
	CTaskComponentUpdate(void)
		: m_indexThread(0)
		, m_pComponentManager(NULL)
	{

	}
	virtual ~CTaskComponentUpdate(void)
	{

	}


public:
	void SetParams(int indexThread, CComponentManager<T> *pComponentManager, float deltaTime)
	{
		m_deltaTime = deltaTime;

		m_indexThread = indexThread;
		m_pComponentManager = pComponentManager;
	}

	virtual void TaskFunc(void *pParams)
	{
		if (m_pComponentManager) {
			uint32_t count = (m_pComponentManager->GetComponentCount() + (THREAD_COUNT - m_pComponentManager->GetComponentCount() % THREAD_COUNT)) / THREAD_COUNT;
			uint32_t indexBegin = max(count * m_indexThread, 0);
			uint32_t indexEnd = min(count * (m_indexThread + 1), m_pComponentManager->GetComponentCount());

			for (uint32_t index = indexBegin; index < indexEnd; index++) {
				CComponent *pComponent = (CComponent *)m_pComponentManager->GetComponentByIndex(index);
				pComponent->Update(m_deltaTime);
			}
		}
	}


private:
	float m_deltaTime;

private:
	int m_indexThread;
	CComponentManager<T> *m_pComponentManager;
};
