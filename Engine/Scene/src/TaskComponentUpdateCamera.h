#pragma once
#include "Engine.h"
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T> class CTaskComponentUpdateCamera : public CTask
{
public:
	CTaskComponentUpdateCamera(void)
		: m_indexThread(0)
		, m_indexQueue(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateCamera(void)
	{

	}


public:
	void SetParams(int indexThread, int indexQueue, CComponentManager<T> *pComponentManager, CGfxCamera *pCamera)
	{
		m_pCamera = pCamera;

		m_indexThread = indexThread;
		m_indexQueue = indexQueue;
		m_pComponentManager = pComponentManager;
	}

	void TaskFunc(void *pParams)
	{
		if (m_pComponentManager) {
			size_t count = (m_pComponentManager->GetComponentCount() + (THREAD_COUNT - m_pComponentManager->GetComponentCount() % THREAD_COUNT)) / THREAD_COUNT;
			size_t indexBegin = max(count * m_indexThread, 0);
			size_t indexEnd = min(count * (m_indexThread + 1), m_pComponentManager->GetComponentCount());

			for (size_t index = indexBegin; index < indexEnd; index++) {
				m_pComponentManager->GetComponentByIndex(index)->TaskUpdateCamera(m_pCamera, m_indexThread, m_indexQueue);
			}
		}
	}


private:
	CGfxCamera *m_pCamera;

private:
	int m_indexThread;
	int m_indexQueue;
	CComponentManager<T> *m_pComponentManager;
};
