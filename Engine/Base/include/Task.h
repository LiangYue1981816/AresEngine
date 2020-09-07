#pragma once
#include "PreHeader.h"


class CTask
{
	friend class CTaskPool;
	friend class CTaskGraph;
	friend class CTaskStack;


public:
	CTask(void);
	virtual ~CTask(void);


public:
	void* GetParam(void) const;

	void SetParam(void* pParam);
	void SetEvent(event_t* pEvent);

	void Signal(void);
	void Unsignal(void);

	void Wait(void);
	void Wait(int msec);

private:
	virtual void TaskFunc(int indexThread) = 0;
	virtual void TaskFinish(void) {};


private:
	void* m_pParam;
	event_t* m_pEvent;

private:
	CTask* pNext;
};


class CTaskLambda : public CTask
{
public:
	template<class Func, class... Args>
	CTaskLambda(Func&& func, Args&& ... args);
	virtual ~CTaskLambda(void);


public:
	virtual void TaskFunc(int indexThread);


private:
	std::function<void()> m_function;
};


class CTaskStack
{
public:
	CTaskStack(void);
	virtual ~CTaskStack(void);


public:
	void Push(CTask* pTask);
	CTask* Pop(void);
	CTask* Head(void) const;
	bool IsEmpty(void) const;


private:
	std::atomic<CTask*> m_pTaskStack;
};
