#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxResource.h"


CGfxResource::CGfxResource(void)
	: refCount(0)
{
	pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
}

CGfxResource::~CGfxResource(void)
{
	pthread_spin_destroy(&lock);
}

uint32_t CGfxResource::IncRefCount(void)
{
	uint32_t count;

	pthread_spin_lock(&lock);
	{
		count = ++refCount;
	}
	pthread_spin_unlock(&lock);

	return count;
}

uint32_t CGfxResource::DecRefCount(void)
{
	uint32_t count;

	pthread_spin_lock(&lock);
	{
		count = --refCount;
	}
	pthread_spin_unlock(&lock);

	return count;
}
