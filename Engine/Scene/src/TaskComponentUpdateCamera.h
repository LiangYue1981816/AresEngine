#pragma once
#include "PreHeader.h"


template<class T>
class CTaskComponentUpdateCamera : public CTask
{
public:
	CTaskComponentUpdateCamera(void)
		: m_mask(0xffffffff)
		, m_bComputeLOD(false)

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
	void SetParams(CComponentManager<T>* pComponentManager, int numThreads, int indexThread, CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD)
	{
		m_mask = mask;
		m_bComputeLOD = bComputeLOD;

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
			m_pComponentManager->GetComponentByIndex(index)->TaskUpdateCamera(m_pCamera, m_pRenderQueue, m_mask, m_bComputeLOD, indexThread);
		}
	}


private:
	uint32_t m_mask;
	bool m_bComputeLOD;

	CGfxCamera* m_pCamera;
	CRenderQueue* m_pRenderQueue;

private:
	int m_indexBegin;
	int m_indexEnd;
	CComponentManager<T>* m_pComponentManager;
};
