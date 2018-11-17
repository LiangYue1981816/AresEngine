#include "GfxRenderer.h"
#include "GfxResource.h"


CGfxResource::CGfxResource(void)
	: refCount(0)
{
	atomic_spin_init(&lock);
}

CGfxResource::~CGfxResource(void)
{

}

uint32_t CGfxResource::IncRefCount(void)
{
	atomic_spin_autolock autolock(&lock);
	return ++refCount;
}

uint32_t CGfxResource::DecRefCount(void)
{
	atomic_spin_autolock autolock(&lock);
	return --refCount;
}
