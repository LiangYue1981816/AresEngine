#include "Utils.h"
#include "TaskPool.h"


CTaskPool::CTaskPool(const char *szName, int numThreads)
	: m_pTaskListHead(nullptr)
{
	if (numThreads == -1) {
		numThreads = NumCpuCores();
	}

	event_init(&m_eventExit, 0);
	atomic_spin_init(&m_lockTaskList);

	m_params.resize(numThreads);
	m_threads.resize(numThreads);

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		char szThreadName[_MAX_STRING];
		sprintf(szThreadName, "%s_%d", szName, indexThread);

		m_params[indexThread].pTaskPool = this;
		m_params[indexThread].indexThread = indexThread;

		pthread_create(&m_threads[indexThread], nullptr, TaskThread, &m_params[indexThread]);
		pthread_set_name(m_threads[indexThread], szThreadName);
	}
}

CTaskPool::~CTaskPool(void)
{
	event_signal(&m_eventExit);

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		pthread_join(m_threads[indexThread], nullptr);
	}

	event_destroy(&m_eventExit);
}

int CTaskPool::GetNumThreads(void) const
{
	return m_threads.size();
}

void CTaskPool::Task(CTask *pTask, void *pParams, event_t *pEventSignal, bool bHighPriority)
{
	pTask->SetTaskParams(pParams);
	pTask->SetTaskEventSignal(pEventSignal);

	atomic_spin_lock(&m_lockTaskList);
	{
		if (bHighPriority == false && m_pTaskListHead) {
			CTask *pLastTask = m_pTaskListHead;

			while (pLastTask->pNext) {
				pLastTask = pLastTask->pNext;
			}

			pTask->pNext = nullptr;
			pLastTask->pNext = pTask;
		}
		else {
			pTask->pNext = m_pTaskListHead;
			m_pTaskListHead = pTask;
		}
	}
	atomic_spin_unlock(&m_lockTaskList);
}

void* CTaskPool::TaskThread(void *pParams)
{
	CTaskPool *pTaskPool = ((ThreadParam *)pParams)->pTaskPool;
	int indexThread = ((ThreadParam *)pParams)->indexThread;

	while (true) {
		// Check if the thread needs to exit
		if (event_wait_timeout(&pTaskPool->m_eventExit, 1) == 0) {
			break;
		}

		// Run tasks
		do {
			bool bFinish = false;
			CTask *pTask = nullptr;

			atomic_spin_lock(&pTaskPool->m_lockTaskList);
			{
				if (pTaskPool->m_pTaskListHead) {
					pTask = pTaskPool->m_pTaskListHead;
					pTaskPool->m_pTaskListHead = pTask->pNext;
				}
				else {
					bFinish = true;
				}
			}
			atomic_spin_unlock(&pTaskPool->m_lockTaskList);

			if (pTask) {
				pTask->TaskFunc(indexThread, pTask->GetTaskParams());
				pTask->SetTaskSignal();
			}

			if (bFinish) {
				break;
			}
		} while (true);
	}

	return nullptr;
}
