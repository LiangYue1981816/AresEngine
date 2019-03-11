#pragma once
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T>
class CTaskComponentUpdateCamera : public CTask
{
public:
	CTaskComponentUpdateCamera(void)
		: m_numThreads(0)
		, m_indexThread(0)
		, m_indexQueue(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateCamera(void)
	{

	}


public:
	void SetParams(int numThreads, int indexThread, int indexQueue, CComponentManager<T> *pComponentManager, CGfxCamera *pCamera)
	{
		m_pCamera = pCamera;

		m_numThreads = numThreads;
		m_indexThread = indexThread;
		m_indexQueue = indexQueue;
		m_pComponentManager = pComponentManager;
	}

	void TaskFunc(uint32_t threadName, void *pParams)
	{
		if (m_pComponentManager) {
			size_t count = (m_pComponentManager->GetComponentCount() + (m_numThreads - m_pComponentManager->GetComponentCount() % m_numThreads)) / m_numThreads;
			size_t indexBegin = std::max(count * m_indexThread, (size_t)0);
			size_t indexEnd = std::min(count * (m_indexThread + 1), m_pComponentManager->GetComponentCount());

			for (size_t index = indexBegin; index < indexEnd; index++) {
				m_pComponentManager->GetComponentByIndex(index)->TaskUpdateCamera(m_pCamera, m_indexThread, m_indexQueue);
			}
		}
	}


private:
	CGfxCamera *m_pCamera;

private:
	int m_numThreads;
	int m_indexThread;
	int m_indexQueue;
	CComponentManager<T> *m_pComponentManager;
};
