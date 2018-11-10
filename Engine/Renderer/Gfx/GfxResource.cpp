#include "GfxRenderer.h"
#include "GfxResource.h"


CGfxResource::CGfxResource(void)
	: refCount(0)
{
	pthread_mutex_init(&lock, nullptr);
}

CGfxResource::~CGfxResource(void)
{
	pthread_mutex_destroy(&lock);
}

uint32_t CGfxResource::IncRefCount(void)
{
	uint32_t count;

	pthread_mutex_lock(&lock);
	{
		count = ++refCount;
	}
	pthread_mutex_unlock(&lock);

	return count;
}

uint32_t CGfxResource::DecRefCount(void)
{
	uint32_t count;

	pthread_mutex_lock(&lock);
	{
		count = --refCount;
	}
	pthread_mutex_unlock(&lock);

	return count;
}
