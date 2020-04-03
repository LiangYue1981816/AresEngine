#pragma once
#include "PreHeader.h"


class CALL_API CTaskPool
{
private:
	typedef struct Thread {
		CTaskPool* pTaskPool;
		int indexThread;

		pthread_t thread;

		event_t eventFinish;
		event_t eventDispatch;
	} Thread;


public:
	CTaskPool(const char* szName, int numThreads = -1);
	virtual ~CTaskPool(void);


public:
	void Task(CTask* pTask, void* pParam, event_t* pEventSignal);

	void Dispatch(void);
	void Wait(void);

private:
	static void* TaskThread(void* pParam);


private:
	bool m_bExit;
	eastl::vector<Thread> m_threads;

private:
	CTaskStack m_tasks;
};
