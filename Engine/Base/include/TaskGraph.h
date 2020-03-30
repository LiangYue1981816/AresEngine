#pragma once
#include "PreHeader.h"


class CALL_API CTaskGraph
{
private:
	typedef struct Thread {
		CTaskGraph* pTaskGraph;
		int indexThread;

		pthread_t thread;

		event_t eventFinish;
		event_t eventDispatch;
	} Thread;


public:
	CTaskGraph(const char* szName, int numThreads = -1);
	virtual ~CTaskGraph(void);


public:
	void Task(CTask* pTask, void* pParam, event_t* pEventSignal, event_t* pEventWait = nullptr);

	void Dispatch(void);
	void Wait(void);

private:
	static void* TaskThread(void* pParam);


private:
	bool m_bExit;
	eastl::vector<Thread> m_threads;

private:
	eastl::unordered_map<event_t*, event_t*> m_pDependence;
	eastl::unordered_map<event_t*, std::atomic<CTask*>> m_pTaskStacks;
};
