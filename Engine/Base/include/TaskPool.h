#pragma once
#include "PreHeader.h"


class CALL_API CTaskPool
{
private:
	typedef struct ThreadParam {
		ThreadParam(void)
		{
			pTaskPool = nullptr;
			indexThread = 0;
		}

		void Set(CTaskPool* _pTaskPool, int _indexThread)
		{
			pTaskPool = _pTaskPool;
			indexThread = _indexThread;
		}

		CTaskPool* pTaskPool;
		int indexThread;
	} ThreadParam;


public:
	CTaskPool(const char* szName, int numThreads = -1);
	virtual ~CTaskPool(void);


public:
	int GetNumThreads(void) const;

public:
	void Task(CTask* pTask, void* pParams, event_t* pEventSignal, bool bHighPriority = false);

private:
	static void* TaskThread(void* pParams);


private:
	event_t m_eventExit;
	eastl::vector<pthread_t> m_threads;
	eastl::vector<ThreadParam> m_params;

private:
	std::atomic_flag m_lockTaskList;
	CTask* m_pTaskListHead;
};
