#pragma once
#include "PreHeader.h"
#include "Lock.h"
#include "Task.h"
#include "event.h"
#include "pthreadex.h"


class CALL_API CTaskGraph
{
public:
	CTaskGraph(const char *szName, int numThreads = THREAD_COUNT);
	virtual ~CTaskGraph(void);


public:
	void Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait = nullptr);
	void Dispatch(void);
	void Wait(void);

private:
	static void* TaskThread(void *pParams);


private:
	event_t m_eventExit;
	event_t m_eventReady;
	event_t m_eventFinish;
	event_t m_eventDispatch;
	eastl::vector<pthread_t> m_threads;

private:
	std::atomic_flag m_lockTaskList;
	eastl::unordered_map<event_t*, CTask*> m_pTaskListHeads;
	eastl::unordered_map<event_t*, event_t*> m_pTaskListDependence;
};
