#include "TaskGraph.h"


#ifdef PLATFORM_WINDOWS
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
	#define MS_VC_EXCEPTION 0x406D1388

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

static void pthread_set_name(pthread_t thread, const char *name)
{
#ifdef PLATFORM_WINDOWS
	if (DWORD Win32ThreadID = pthread_getw32threadid_np(thread)) {
		SetThreadName(Win32ThreadID, name);
	}
#elif PLATFORM_ANDROID
	pthread_setname_np(thread, name);
#endif
}

static int pthread_attr_create(pthread_attr_t *attr, sched_param *param, int policy, int priority)
{
#ifdef PLATFORM_WINDOWS
	policy = SCHED_OTHER;
#endif

	int rcode = NO_ERROR;

	const int min_priority = 1;
	const int max_priority = 99;
	if (priority < min_priority) priority = min_priority;
	if (priority > max_priority) priority = max_priority;

	const int sched_min_priority = sched_get_priority_min(policy);
	const int sched_max_priority = sched_get_priority_max(policy);
	param->sched_priority = (sched_max_priority - sched_min_priority) * priority / (max_priority - min_priority) + sched_min_priority;

	rcode = pthread_attr_init(attr);
	if (rcode != NO_ERROR) goto RET;

	rcode = pthread_attr_setschedpolicy(attr, policy);
	if (rcode != NO_ERROR) goto RET;

	rcode = pthread_attr_setschedparam(attr, param);
	if (rcode != NO_ERROR) goto RET;

RET:
	return rcode;
}


static void atomic_spin_init(std::atomic_flag *flag)
{
	flag->clear();
}

static void atomic_spin_lock(std::atomic_flag *flag)
{
	while (flag->test_and_set(std::memory_order_acquire));
}

static void atomic_spin_unlock(std::atomic_flag *flag)
{
	flag->clear(std::memory_order_release);
}



CTaskGraph::CTaskGraph(const char *szName)
{
	event_init(&m_eventExit, 0);
	event_init(&m_eventReady, 1);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	atomic_spin_init(&m_lockTaskList);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		char szThreadName[_MAX_STRING];
		sprintf(szThreadName, "%s_%d", szName, indexThread);
		pthread_create(&m_threads[indexThread], nullptr, TaskThread, this);
		pthread_set_name(m_threads[indexThread], szThreadName);
	}
}

CTaskGraph::~CTaskGraph(void)
{
	event_signal(&m_eventExit);
	event_signal(&m_eventDispatch);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		pthread_join(m_threads[indexThread], nullptr);
	}

	event_destroy(&m_eventExit);
	event_destroy(&m_eventReady);
	event_destroy(&m_eventFinish);
	event_destroy(&m_eventDispatch);
}

void CTaskGraph::Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait)
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
			event_t *pEvent = nullptr;
			do {
				if (pEvent) {
					event_wait(pEvent);
				}

				if (CTask **ppTaskListHead = &pTaskGraph->m_pTaskListHeads[pEvent]) {
					while (true) {
						bool bFinish = false;
						CTask *pTask = nullptr;

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
							pTask->TaskFunc(pTask->GetTaskParams());
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
