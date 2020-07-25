#pragma once
#include "PreHeader.h"


class mutex_autolock
{
public:
	mutex_autolock(pthread_mutex_t* mutex);
	~mutex_autolock(void);


private:
	pthread_mutex_t* m_mutex;
};

class atomic_spin_autolock
{
public:
	atomic_spin_autolock(std::atomic_flag* flag);
	~atomic_spin_autolock(void);


private:
	std::atomic_flag* m_flag;
};

CALL_API void atomic_spin_init(std::atomic_flag* flag);
CALL_API void atomic_spin_lock(std::atomic_flag* flag);
CALL_API void atomic_spin_unlock(std::atomic_flag* flag);
