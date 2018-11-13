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
	mutex_autolock mutex(&lock);
	return ++refCount;
}

uint32_t CGfxResource::DecRefCount(void)
{
	mutex_autolock mutex(&lock);
	return --refCount;
}
