#include "Task.h"


CTask::CTask(void)
	: m_pTaskParams(nullptr)
	, m_pTaskEventSignal(nullptr)

	, pNext(nullptr)
{

}

CTask::~CTask(void)
{

}

void CTask::SetTaskSignal(void)
{
	if (m_pTaskEventSignal) {
		event_signal(m_pTaskEventSignal);
	}
}

void CTask::SetTaskEventSignal(event_t* pEventSignal)
{
	m_pTaskEventSignal = pEventSignal;

	if (m_pTaskEventSignal) {
		event_unsignal(m_pTaskEventSignal);
	}
}

void CTask::SetTaskParams(void* pParams)
{
	m_pTaskParams = pParams;
}

void* CTask::GetTaskParams(void) const
{
	return m_pTaskParams;
}
