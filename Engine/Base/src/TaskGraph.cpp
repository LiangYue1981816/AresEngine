#include "BaseHeader.h"


CTaskGraph::CTaskGraph(const char* szName, int numThreads)
{
	if (numThreads == -1) {
		numThreads = NumCpuCores();
	}

	if (numThreads > MAX_THREAD_COUNT) {
		numThreads = MAX_THREAD_COUNT;
	}

	m_bExit = false;

	event_init(&m_eventReady, 1);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	atomic_spin_init(&m_lockTaskList);

	m_params.resize(numThreads);
	m_threads.resize(numThreads);

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		char szThreadName[_MAX_STRING];
		sprintf(szThreadName, "%s_%d", szName, indexThread);

		m_params[indexThread].pTaskGraph = this;
		m_params[indexThread].indexThread = indexThread;

		pthread_create(&m_threads[indexThread], nullptr, TaskThread, &m_params[indexThread]);
		pthread_set_name(m_threads[indexThread], szThreadName);
	}
}

CTaskGraph::~CTaskGraph(void)
{
	m_bExit = true;

	event_signal(&m_eventDispatch);

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		pthread_join(m_threads[indexThread], nullptr);
	}

	event_destroy(&m_eventReady);
	event_destroy(&m_eventFinish);
	event_destroy(&m_eventDispatch);
}

int CTaskGraph::GetNumThreads(void) const
{
	return m_threads.size();
}

void CTaskGraph::Task(CTask* pTask, void* pParams, event_t* pEventSignal, event_t* pEventWait)
{
	pTask->SetTaskParams(pParams);
	pTask->SetTaskEventSignal(pEventSignal);

	atomic_spin_lock(&m_lockTaskList);
	{
		pTask->pNext = m_pTaskListHeads[pEventWait];
		m_pTaskListHeads[pEventWait] = pTask;
		m_pTaskListDependence[pEventWait] = pEventSignal;
	}
	atomic_spin_unlock(&m_lockTaskList);
}

void CTaskGraph::Dispatch(void)
{
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_unsignal(&m_eventReady);
		event_unsignal(&m_eventFinish);
	}

	event_signal(&m_eventDispatch);
}

void CTaskGraph::Wait(void)
{
	event_wait(&m_eventFinish);

	m_pTaskListHeads.clear();
	m_pTaskListDependence.clear();
}

void* CTaskGraph::TaskThread(void* pParams)
{
	CTaskGraph* pTaskGraph = ((ThreadParam*)pParams)->pTaskGraph;
	int indexThread = ((ThreadParam*)pParams)->indexThread;

	while (true) {
		event_wait(&pTaskGraph->m_eventDispatch);
		{
			// Check if the thread needs to exit
			if (pTaskGraph->m_bExit) {
				break;
			}

			// Waiting for all threads ready
			event_signal(&pTaskGraph->m_eventReady);
			event_wait(&pTaskGraph->m_eventReady);

			// Run tasks
			event_t* pEvent = nullptr;
			do {
				if (pEvent) {
					event_wait(pEvent);
				}

				if (CTask** ppTaskListHead = &pTaskGraph->m_pTaskListHeads[pEvent]) {
					while (true) {
						bool bFinish = false;
						CTask* pTask = nullptr;

						atomic_spin_lock(&pTaskGraph->m_lockTaskList);
						{
							if (*ppTaskListHead) {
								pTask = *ppTaskListHead;
								*ppTaskListHead = pTask->pNext;
							}
							else {
								bFinish = true;
							}
						}
						atomic_spin_unlock(&pTaskGraph->m_lockTaskList);

						if (pTask) {
							pTask->TaskFunc(indexThread, pTask->GetTaskParams());
							pTask->SetTaskSignal();
						}

						if (bFinish) {
							break;
						}
					}
				}
			} while ((pEvent = pTaskGraph->m_pTaskListDependence[pEvent]) != nullptr);
		}
		event_reset(&pTaskGraph->m_eventDispatch);
		event_signal(&pTaskGraph->m_eventFinish);
	}

	return nullptr;
}
