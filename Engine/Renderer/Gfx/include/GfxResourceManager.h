#pragma once
#include "PreHeader.h"


class CALL_API CGfxResourceManager
{
public:
	CGfxResourceManager(void);
	virtual ~CGfxResourceManager(void);


protected:
	pthread_mutex_t lock;
};
