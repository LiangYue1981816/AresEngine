#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"


class CALL_API mutex_autolock
{
public:
	mutex_autolock(pthread_mutex_t *mutex);
	~mutex_autolock(void);


private:
	pthread_mutex_t *m_mutex;
};
