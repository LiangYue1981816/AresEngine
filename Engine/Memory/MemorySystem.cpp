#include <stdlib.h>
#include <assert.h>


void* _malloc(size_t size)
{
	void *ptr = nullptr;

#ifdef PLATFORM_WINDOWS

	ptr = _aligned_malloc(size, 16);

#elif PLATFORM_ANDROID

	ptr = memalign(16, size);

#else

	ptr = malloc(size);

#endif

	return ptr;
}

void _free(void *ptr)
{
#ifdef PLATFORM_WINDOWS

	_aligned_free(ptr);

#else

	free(ptr);

#endif
}
