#pragma once
#include "TaskGraph.h"
#include "Component.h"
#include "ComponentManager.h"


template<class T>
class CTaskComponentUpdateCamera : public CTask
{
public:
	CTaskComponentUpdateCamera(void)
		: m_mask(0xffffffff)
		, m_pCamera(nullptr)
		, m_pRenderQueue(nullptr)

		, m_indexBegin(0)
		, m_indexEnd(0)
		, m_pComponentManager(nullptr)
	{

	}
	virtual ~CTaskComponentUpdateCamera(void)
	{

	}


public:
	void SetParams(CComponentManager<T>* pComponentManager, int numThreads, int indexThread, CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask)
	{
		m_mask = mask;
		m_pCamera = pCamera;
		m_pRenderQueue = pRenderQueue;

		int count = (pComponentManager->GetComponentCount() + (numThreads - pComponentManager->GetComponentCount() % numThreads)) / numThreads;
		m_indexBegin = std::max(count * indexThread, 0);
		m_indexEnd = std::min(count * (indexThread + 1), (int)pComponentManager->GetComponentCount());
		m_pComponentManager = pComponentManager;
	}

	void TaskFunc(int indexThread, void* pParams)
	{
		for (int index = m_indexBegin; index < m_indexEnd; index++) {
			m_pComponentManager->GetComponentByIndex(index)->TaskUpdateCamera(m_pCamera, m_pRenderQueue, m_mask, indexThread);
		}
	}


private:
	uint32_t m_mask;
	CGfxCamera* m_pCamera;
	CGfxRenderQueue* m_pRenderQueue;

private:
	int m_indexBegin;
	int m_indexEnd;
	CComponentManager<T>* m_pComponentManager;
};
