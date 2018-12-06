#pragma once
#include "PreHeader.h"
#include "event.h"
#include "pthread.h"


class CALL_API CTask
{
	friend class CTaskGraph;


public:
	CTask(void);
	virtual ~CTask(void);


public:
	virtual void TaskFunc(void *pParams) = 0;

private:
	inline void SetTaskSignal(void);
	inline void SetTaskEventSignal(event_t *pEventSignal);

	inline void SetTaskParams(void *pParams);
	inline void* GetTaskParams(void) const;


private:
	void *m_pTaskParams;
	event_t *m_pTaskEventSignal;

private:
	CTask *pNext;
};


class CALL_API CTaskGraph
{
public:
	CTaskGraph(const char *szName);
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
	pthread_t m_threads[THREAD_COUNT];

private:
	std::atomic_flag m_lockTaskList;
	eastl::unordered_map<event_t*, CTask*> m_pTaskListHeads;
	eastl::unordered_map<event_t*, event_t*> m_pTaskListDependence;
};
