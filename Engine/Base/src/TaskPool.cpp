#include "TaskPool.h"


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



CTaskPool::CTaskPool(const char *szName, int numThreads)
{
	event_init(&m_eventExit, 0);
	atomic_spin_init(&m_lockTaskList);

	m_threads.resize(numThreads);
	for (int indexThread = 0; indexThread < m_threads.size(); indexThread++) {
		char szThreadName[_MAX_STRING];
		sprintf(szThreadName, "%s_%d", szName, indexThread);
		pthread_create(&m_threads[indexThread], nullptr, TaskThread, this);
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

void CTaskPool::Task(CTask *pTask, void *pParams, event_t *pEventSignal)
{
	pTask->SetTaskParams(pParams);
	pTask->SetTaskEventSignal(pEventSignal);

	atomic_spin_lock(&m_lockTaskList);
	{
		pTask->pNext = m_pTaskListHead;
		m_pTaskListHead = pTask;
	}
	atomic_spin_unlock(&m_lockTaskList);
}

void* CTaskPool::TaskThread(void *pParams)
{
	CTaskPool *pTaskPool = (CTaskPool *)pParams;

	while (true) {
		// Check if the thread needs to exit
		if (event_wait_timeout(&pTaskPool->m_eventExit, 0) == 0) {
			break;
		}

		// Run tasks
		CTask *pTask = nullptr;
		atomic_spin_lock(&pTaskPool->m_lockTaskList);
		{
			if (pTaskPool->m_pTaskListHead) {
				pTask = pTaskPool->m_pTaskListHead;
				pTaskPool->m_pTaskListHead = pTask->pNext;
			}
		}
		atomic_spin_unlock(&pTaskPool->m_lockTaskList);

		if (pTask) {
			pTask->TaskFunc(pTask->GetTaskParams());
			pTask->SetTaskSignal();
		}
	}

	return nullptr;
}
