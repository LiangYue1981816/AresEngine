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
	m_threads.resize(numThreads);

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		char szThreadName[_MAX_STRING];
		sprintf(szThreadName, "%s_%d", szName, indexThread);

		m_threads[indexThread].pTaskGraph = this;
		m_threads[indexThread].indexThread = indexThread;

		event_init(&m_threads[indexThread].eventFinish, 1);
		event_init(&m_threads[indexThread].eventDispatch, 0);

		pthread_create(&m_threads[indexThread].thread, nullptr, TaskThread, &m_threads[indexThread]);
		pthread_set_name(m_threads[indexThread].thread, szThreadName);
	}
}

CTaskGraph::~CTaskGraph(void)
{
	m_bExit = true;

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_signal(&m_threads[indexThread].eventDispatch);
		pthread_join(m_threads[indexThread].thread, nullptr);

		event_destroy(&m_threads[indexThread].eventFinish);
		event_destroy(&m_threads[indexThread].eventDispatch);
	}
}

void CTaskGraph::Task(CTask* pTask, void* pParam, event_t* pEventSignal, event_t* pEventWait)
{
	pTask->SetParam(pParam);
	pTask->SetEvent(pEventSignal);
	pTask->Unsignal();

	pTask->pNext = m_pTaskStacks[pEventWait];
	m_pTaskStacks[pEventWait] = pTask;
	m_pDependence[pEventWait] = pEventSignal;
}

void CTaskGraph::Dispatch(void)
{
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_reset(&m_threads[indexThread].eventFinish);
		event_signal(&m_threads[indexThread].eventDispatch);
	}
}

void CTaskGraph::Wait(void)
{
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_wait(&m_threads[indexThread].eventFinish);
	}

	m_pTaskStacks.clear();
	m_pDependence.clear();
}

void* CTaskGraph::TaskThread(void* pParam)
{
	Thread* pThread = (Thread*)pParam;

	while (true) {
		event_wait(&pThread->eventDispatch);
		event_reset(&pThread->eventDispatch);
		{
			if (pThread->pTaskGraph->m_bExit) {
				goto RET;
			}

			event_reset(&pThread->eventFinish);
			{
				event_t* pEvent = nullptr;
				do {
					if (pEvent) {
						event_wait(pEvent);
					}

					if (pThread->pTaskGraph->m_pTaskStacks.find(pEvent) == pThread->pTaskGraph->m_pTaskStacks.end()) {
						break;
					}

					do {
						CTask* pTaskNext = nullptr;
						CTask* pTaskHead = pThread->pTaskGraph->m_pTaskStacks[pEvent].load(std::memory_order_relaxed);
						do {
							if (pTaskHead) {
								pTaskNext = pTaskHead->pNext;
							}
							else {
								break;
							}
						} while (!pThread->pTaskGraph->m_pTaskStacks[pEvent].compare_exchange_weak(pTaskHead, pTaskNext, std::memory_order_release, std::memory_order_relaxed));

						if (pTaskHead) {
							pTaskHead->TaskFunc(pThread->indexThread);
							pTaskHead->Signal();
						}
						else {
							break;
						}
					} while (true);
				} while (pEvent = pThread->pTaskGraph->m_pDependence[pEvent]);
			}
			event_signal(&pThread->eventFinish);
		}
	}
RET:
	return nullptr;
}
