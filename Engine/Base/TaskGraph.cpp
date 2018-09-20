#include "TaskGraph.h"


#ifdef _WINDOWS
#include <windows.h>
#define MS_VC_EXCEPTION 0x406D1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;     // Must be 0x1000.
	LPCSTR szName;    // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

static void SetThreadName(DWORD dwThreadID, const char* szName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
}
#endif

static void pthread_set_name(pthread_t thread, const char *szName)
{
#ifdef _WINDOWS
	if (DWORD Win32ThreadID = pthread_getw32threadid_np(thread)) {
		SetThreadName(Win32ThreadID, szName);
	}
#else
	pthread_setname_np(thread, name);
#endif
}

static pthread_attr_t pthread_attr_create(int priority)
{
	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	int status, thread_policy, rr_min_priority, rr_max_priority;

	pthread_attr_init(&thread_attr);
	pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
	pthread_attr_getschedparam(&thread_attr, &thread_param);
	status = pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
	rr_min_priority = sched_get_priority_min(SCHED_RR);
	rr_max_priority = sched_get_priority_max(SCHED_RR);
	pthread_attr_setschedparam(&thread_attr, &thread_param);
	pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);

	return thread_attr;
}


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



CTaskGraph::CTaskGraph(const char *szName, int priority)
	: m_priority(priority)
{
	if (szName) {
		strcpy(m_szName, szName);
	}
	else {
		strcpy(m_szName, "TaskGraph");
	}

	event_init(&m_eventExit, 0);
	event_init(&m_eventReady, 1);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	pthread_mutex_init(&m_mutexTaskList, NULL);

	pthread_attr_t thread_attr = pthread_attr_create(priority);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		char szName[260];
		sprintf(szName, "%s_%d(%2.2d)", m_szName, indexThread, m_priority);
		pthread_create(&m_threads[indexThread], NULL, TaskThread, this);
		pthread_set_name(m_threads[indexThread], szName);
	}

	pthread_attr_destroy(&thread_attr);
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
