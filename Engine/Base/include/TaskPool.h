#pragma once
#include "PreHeader.h"
#include "Lock.h"
#include "Task.h"
#include "event.h"
#include "pthreadex.h"


class CALL_API CTaskPool
{
private:
	typedef struct ThreadParam {
		CTaskPool *pTaskPool;
		char szThreadName[260];
	} ThreadParam;


public:
	CTaskPool(const char *szName, int numThreads = THREAD_COUNT);
	virtual ~CTaskPool(void);


public:
	int GetNumThreads(void) const;

public:
	void Task(CTask *pTask, void *pParams, event_t *pEventSignal, bool bHighPriority = false);

private:
	static void* TaskThread(void *pParams);


private:
	event_t m_eventExit;
	eastl::vector<pthread_t> m_threads;
	eastl::vector<ThreadParam> m_params;

private:
	std::atomic_flag m_lockTaskList;
	CTask *m_pTaskListHead;
};
