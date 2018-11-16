#include "Mutex.h"


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
