#include "BaseHeader.h"


CTask::CTask(void)
	: m_pParam(nullptr)
	, m_pEvent(nullptr)

	, pNext(nullptr)
{

}

CTask::~CTask(void)
{

}

void* CTask::GetParam(void) const
{
	return m_pParam;
}

void CTask::SetParam(void* pParam)
{
	m_pParam = pParam;
}

void CTask::SetEvent(event_t* pEvent)
{
	m_pEvent = pEvent;
}

void CTask::Signal(void)
{
	if (m_pEvent) {
		event_signal(m_pEvent);
	}
}

void CTask::Unsignal(void)
{
	if (m_pEvent) {
		event_unsignal(m_pEvent);
	}
}

void CTask::Wait(void)
{
	if (m_pEvent) {
		event_wait(m_pEvent);
	}
}

void CTask::Wait(int msec)
{
	if (m_pEvent) {
		event_wait_timeout(m_pEvent, msec);
	}
}


template<class Func, class... Args>
CTaskLambda::CTaskLambda(Func&& func, Args&& ... args)
{
	m_function = [=]() { func(args...); };
}

CTaskLambda::~CTaskLambda(void)
{

}

void CTaskLambda::TaskFunc(int indexThread)
{
	m_function();
}


CTaskStack::CTaskStack(void)
	: m_pTaskStack(nullptr)
{

}

CTaskStack::~CTaskStack(void)
{

}

void CTaskStack::Push(CTask* pTask)
{
	CTask* pTaskHead = m_pTaskStack.load(std::memory_order_acquire);
	{
		do {
			pTask->pNext = pTaskHead;
		} while (!m_pTaskStack.compare_exchange_weak(pTaskHead, pTask, std::memory_order_release, std::memory_order_acquire));
	}
}

CTask* CTaskStack::Pop(void)
{
	CTask* pTaskHead = m_pTaskStack.load(std::memory_order_acquire);
	{
		do {
			if (pTaskHead == nullptr) {
				break;
			}
		} while (!m_pTaskStack.compare_exchange_weak(pTaskHead, pTaskHead->pNext, std::memory_order_release, std::memory_order_acquire));
	}
	return pTaskHead;
}

CTask* CTaskStack::Head(void) const
{
	return m_pTaskStack.load(std::memory_order_acquire);
}

bool CTaskStack::IsEmpty(void) const
{
	return Head() == nullptr;
}
