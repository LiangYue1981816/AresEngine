#pragma once
#include "PreHeader.h"


class CALL_API CTask
{
	friend class CTaskPool;
	friend class CTaskGraph;
	friend class CTaskStack;


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


class CALL_API CTaskStack
{
public:
	CTaskStack(void)
		: m_pTaskStack(nullptr)
	{

	}
	CTaskStack(const CTaskStack& copy)
		: m_pTaskStack(copy.Head())
	{

	}
	virtual ~CTaskStack(void)
	{

	}


public:
	void Push(CTask* pTask)
	{
		CTask* pTaskHead = m_pTaskStack.load(std::memory_order_relaxed);

		do {
			pTask->pNext = pTaskHead;
		} while (!m_pTaskStack.compare_exchange_weak(pTaskHead, pTask, std::memory_order_release, std::memory_order_relaxed));
	}

	CTask* Pop(void)
	{
		CTask* pTaskNext = nullptr;
		CTask* pTaskHead = m_pTaskStack.load(std::memory_order_relaxed);

		do {
			if (pTaskHead) {
				pTaskNext = pTaskHead->pNext;
			}
			else {
				break;
			}
		} while (!m_pTaskStack.compare_exchange_weak(pTaskHead, pTaskNext, std::memory_order_release, std::memory_order_relaxed));

		return pTaskHead;
	}

	CTask* Head(void) const
	{
		return m_pTaskStack.load(std::memory_order_relaxed);
	}


private:
	std::atomic<CTask*> m_pTaskStack;
};
