#pragma once
#include "PreHeader.h"
#include "Lock.h"
#include "Task.h"
#include "event.h"
#include "pthreadex.h"


class CALL_API CTaskGraph
{
private:
	typedef struct ThreadParam {
		CTaskGraph *pTaskGraph;
		int indexThread;
	} ThreadParam;


public:
	CTaskGraph(const char *szName, int numThreads = -1);
	virtual ~CTaskGraph(void);


public:
	int GetNumThreads(void) const;

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
	eastl::vector<ThreadParam> m_params;

private:
	std::atomic_flag m_lockTaskList;
	eastl::unordered_map<event_t*, CTask*> m_pTaskListHeads;
	eastl::unordered_map<event_t*, event_t*> m_pTaskListDependence;
};
