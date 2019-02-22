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


class CALL_API CTaskLambda : public CTask
{
public:
	template<class Func, class... Args>
	CTaskLambda(Func&& func, Args&&... args)
	{
		m_function = [=]() { func(args...); };
	}
	virtual ~CTaskLambda(void)
	{

	}


public:
	virtual void TaskFunc(void *pParams)
	{
		m_function();
	}


private:
	std::function<void()> m_function;
};
