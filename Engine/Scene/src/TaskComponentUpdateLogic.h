#pragma once
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T>
class CTaskComponentUpdateLogic : public CTask
{
public:
	CTaskComponentUpdateLogic(void)
		: m_numThreads(0)
		, m_indexThread(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateLogic(void)
	{

	}


public:
	void SetParams(int numThreads, int indexThread, CComponentManager<T> *pComponentManager, float gameTime, float deltaTime)
	{
		m_gameTime = gameTime;
		m_deltaTime = deltaTime;

		m_numThreads = numThreads;
		m_indexThread = indexThread;
		m_pComponentManager = pComponentManager;
	}

	void TaskFunc(uint32_t threadName, void *pParams)
	{
		if (m_pComponentManager) {
			size_t count = (m_pComponentManager->GetComponentCount() + (m_numThreads - m_pComponentManager->GetComponentCount() % m_numThreads)) / m_numThreads;
			size_t indexBegin = std::max(count * m_indexThread, (size_t)0);
			size_t indexEnd = std::min(count * (m_indexThread + 1), m_pComponentManager->GetComponentCount());

			for (size_t index = indexBegin; index < indexEnd; index++) {
				m_pComponentManager->GetComponentByIndex(index)->TaskUpdate(m_gameTime, m_deltaTime);
			}
		}
	}


private:
	float m_gameTime;
	float m_deltaTime;

private:
	int m_numThreads;
	int m_indexThread;
	CComponentManager<T> *m_pComponentManager;
};
