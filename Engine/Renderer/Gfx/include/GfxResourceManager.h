#pragma once
#include "PreHeader.h"


class CGfxResourceManager
{
public:
	CGfxResourceManager(void);
	virtual ~CGfxResourceManager(void);


protected:
	pthread_mutex_t lock;
};
