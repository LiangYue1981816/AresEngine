#pragma once
#include "PreHeader.h"


class CALL_API CTask
{
	friend class CTaskPool;
	friend class CTaskGraph;


public:
	CTask(void)
		: m_pParam(nullptr)
		, m_pEvent(nullptr)

		, pNext(nullptr)
	{

	}
	virtual ~CTask(void)
	{

	}


public:
	void SetParam(void* pParam)
	{
		m_pParam = pParam;
	}

	void SetEvent(event_t* pEvent)
	{
		m_pEvent = pEvent;
	}

	void Signal(void)
	{
		if (m_pEvent) {
			event_signal(m_pEvent);
		}
	}

	void Unsignal(void)
	{
		if (m_pEvent) {
			event_unsignal(m_pEvent);
		}
	}

	void Wait(void)
	{
		if (m_pEvent) {
			event_wait(m_pEvent);
		}
	}

	void Wait(int msec)
	{
		if (m_pEvent) {
			event_wait_timeout(m_pEvent, msec);
		}
	}

private:
	virtual void TaskFunc(int indexThread)
	{

	}


private:
	void* m_pParam;
	event_t* m_pEvent;

private:
	CTask* pNext;
};


class CALL_API CTaskLambda : public CTask
{
public:
	template<class Func, class... Args>
	CTaskLambda(Func&& func, Args&& ... args)
	{
		m_function = [=]() { func(args...); };
	}
	virtual ~CTaskLambda(void)
	{

	}


public:
	virtual void TaskFunc(int indexThread)
	{
		m_function();
	}


private:
	std::function<void()> m_function;
};
