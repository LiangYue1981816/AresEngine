#include "Lock.h"


mutex_autolock::mutex_autolock(pthread_mutex_t *mutex)
	: m_mutex(mutex)
{
	if (m_mutex) {
		pthread_mutex_lock(m_mutex);
	}	
}

mutex_autolock::~mutex_autolock(void)
{
	if (m_mutex) {
		pthread_mutex_unlock(m_mutex);
	}
}

atomic_spin_autolock::atomic_spin_autolock(std::atomic_flag *flag)
	: m_flag(flag)
{
	if (m_flag) {
		atomic_spin_lock(m_flag);
	}
}

atomic_spin_autolock::~atomic_spin_autolock(void)
{
	if (m_flag) {
		atomic_spin_unlock(m_flag);
	}
}

CALL_API void atomic_spin_init(std::atomic_flag *flag)
{
	if (flag) {
		flag->clear();
	}
}

CALL_API void atomic_spin_lock(std::atomic_flag *flag)
{
	if (flag) {
		while (flag->test_and_set(std::memory_order_acquire));
	}
}

CALL_API void atomic_spin_unlock(std::atomic_flag *flag)
{
	if (flag) {
		flag->clear(std::memory_order_release);
	}
}
