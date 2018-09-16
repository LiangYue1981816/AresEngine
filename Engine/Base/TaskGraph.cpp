#include "TaskGraph.h"


CTask::CTask(void)
	: m_pTaskParams(NULL)
	, m_pTaskEventSignal(NULL)

	, pNext(NULL)
{

}

CTask::~CTask(void)
{

}

void CTask::SetTaskSignal(void)
{
	if (m_pTaskEventSignal) {
		event_signal(m_pTaskEventSignal);
	}
}

void CTask::SetTaskEventSignal(event_t *pEventSignal)
{
	m_pTaskEventSignal = pEventSignal;

	if (m_pTaskEventSignal) {
		event_unsignal(m_pTaskEventSignal);
	}
}

void CTask::SetTaskParams(void *pParams)
{
	m_pTaskParams = pParams;
}

void* CTask::GetTaskParams(void) const
{
	return m_pTaskParams;
}



CTaskGraph::CTaskGraph(void)
{
	event_init(&m_eventExit, 0);
	event_init(&m_eventReady, 1);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	pthread_mutex_init(&m_mutexTaskList, NULL);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		pthread_create(&m_threads[indexThread], NULL, TaskThread, this);
	}
}

CTaskGraph::~CTaskGraph(void)
{
	event_signal(&m_eventExit);
	event_signal(&m_eventDispatch);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		pthread_join(m_threads[indexThread], NULL);
	}

	event_destroy(&m_eventExit);
	event_destroy(&m_eventReady);
	event_destroy(&m_eventFinish);
	event_destroy(&m_eventDispatch);
	pthread_mutex_destroy(&m_mutexTaskList);
}

void CTaskGraph::Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait)
{
	pTask->SetTaskParams(pParams);
	pTask->SetTaskEventSignal(pEventSignal);

	pthread_mutex_lock(&m_mutexTaskList);
	{
		pTask->pNext = m_pTaskListHeads[pEventWait];
		m_pTaskListHeads[pEventWait] = pTask;
		m_pTaskListDependence[pEventWait] = pEventSignal;
	}
	pthread_mutex_unlock(&m_mutexTaskList);
}

void CTaskGraph::Dispatch(void)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
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

void* CTaskGraph::TaskThread(void *pParams)
{
	CTaskGraph *pTaskGraph = (CTaskGraph *)pParams;

	while (true) {
		event_wait(&pTaskGraph->m_eventDispatch);
		{
			// Check if the thread needs to exit
			if (event_wait_timeout(&pTaskGraph->m_eventExit, 0) == 0) {
				break;
			}

			// Waiting for all threads ready
			event_signal(&pTaskGraph->m_eventReady);
			event_wait(&pTaskGraph->m_eventReady);

			// Run tasks
			event_t *pEvent = NULL;
			do {
				if (pEvent) {
					event_wait(pEvent);
				}

				if (CTask **ppTaskListHead = &pTaskGraph->m_pTaskListHeads[pEvent]) {
					while (true) {
						bool bFinish = false;
						CTask *pTask = NULL;

						pthread_mutex_lock(&pTaskGraph->m_mutexTaskList);
						{
							if (*ppTaskListHead) {
								pTask = *ppTaskListHead;
								*ppTaskListHead = pTask->pNext;
							}
							else {
								bFinish = true;
							}
						}
						pthread_mutex_unlock(&pTaskGraph->m_mutexTaskList);

						if (pTask) {
							pTask->TaskFunc(pTask->GetTaskParams());
							pTask->SetTaskSignal();
						}

						if (bFinish) {
							break;
						}
					}
				}
			} while (pEvent = pTaskGraph->m_pTaskListDependence[pEvent]);
		}
		event_reset(&pTaskGraph->m_eventDispatch);
		event_signal(&pTaskGraph->m_eventFinish);
	}

	return NULL;
}
