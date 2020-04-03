#include "BaseHeader.h"


CTaskPool::CTaskPool(const char* szName, int numThreads)
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

		m_threads[indexThread].pTaskPool = this;
		m_threads[indexThread].indexThread = indexThread;

		event_init(&m_threads[indexThread].eventFinish, 1);
		event_init(&m_threads[indexThread].eventDispatch, 0);

		pthread_create(&m_threads[indexThread].thread, nullptr, TaskThread, &m_threads[indexThread]);
		pthread_set_name(m_threads[indexThread].thread, szThreadName);
	}
}

CTaskPool::~CTaskPool(void)
{
	m_bExit = true;

	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_signal(&m_threads[indexThread].eventDispatch);
		pthread_join(m_threads[indexThread].thread, nullptr);

		event_destroy(&m_threads[indexThread].eventFinish);
		event_destroy(&m_threads[indexThread].eventDispatch);
	}
}

int CTaskPool::GetNumThreads(void) const
{
	return m_threads.size();
}

void CTaskPool::Task(CTask* pTask, void* pParam, event_t* pEventSignal)
{
	pTask->SetParam(pParam);
	pTask->SetEvent(pEventSignal);
	pTask->Unsignal();

	m_tasks.Push(pTask);
}

void CTaskPool::Dispatch(void)
{
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_reset(&m_threads[indexThread].eventFinish);
		event_signal(&m_threads[indexThread].eventDispatch);
	}
}

void CTaskPool::Wait(void)
{
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		event_wait(&m_threads[indexThread].eventFinish);
	}
}

void* CTaskPool::TaskThread(void* pParam)
{
	Thread* pThread = (Thread*)pParam;

	while (true) {
		event_wait(&pThread->eventDispatch);
		event_reset(&pThread->eventDispatch);
		{
			if (pThread->pTaskPool->m_bExit) {
				goto RET;
			}

			event_reset(&pThread->eventFinish);
			{
				do {
					if (CTask* pTask = pThread->pTaskPool->m_tasks.Pop()) {
						pTask->TaskFunc(pThread->indexThread);
						pTask->Signal();
					}
					else {
						break;
					}
				} while (true);
			}
			event_signal(&pThread->eventFinish);
		}
	}
RET:
	return nullptr;
}
