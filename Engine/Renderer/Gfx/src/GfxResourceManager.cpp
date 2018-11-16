#include "GfxRenderer.h"
#include "GfxResourceManager.h"


CGfxResourceManager::CGfxResourceManager(void)
{
	pthread_mutex_init(&lock, nullptr);
}

CGfxResourceManager::~CGfxResourceManager(void)
{
	pthread_mutex_destroy(&lock);
}
