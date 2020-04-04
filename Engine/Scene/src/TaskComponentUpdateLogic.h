#pragma once
#include "PreHeader.h"


template<class T>
class CTaskComponentUpdateLogic : public CTask
{
public:
	CTaskComponentUpdateLogic(void)
		: m_gameTime(0.0f)
		, m_deltaTime(0.0f)

		, m_indexBegin(0)
		, m_indexEnd(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateLogic(void)
	{

	}


public:
	void SetParams(CComponentManager<T>* pComponentManager, int numThreads, int indexThread, float gameTime, float deltaTime)
	{
		m_gameTime = gameTime;
		m_deltaTime = deltaTime;

		int count = (pComponentManager->GetComponentCount() + (numThreads - pComponentManager->GetComponentCount() % numThreads)) / numThreads;
		m_indexBegin = std::max(count * indexThread, 0);
		m_indexEnd = std::min(count * (indexThread + 1), (int)pComponentManager->GetComponentCount());
		m_pComponentManager = pComponentManager;
	}

	void TaskFunc(int indexThread)
	{
		for (int index = m_indexBegin; index < m_indexEnd; index++) {
			m_pComponentManager->GetComponentByIndex(index)->TaskUpdate(m_gameTime, m_deltaTime);
		}
	}


private:
	float m_gameTime;
	float m_deltaTime;

private:
	int m_indexBegin;
	int m_indexEnd;
	CComponentManager<T>* m_pComponentManager;
};
