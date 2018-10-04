#include <stdlib.h>
#include <assert.h>


#ifndef ASSERT
#define ASSERT assert
#endif


void* _malloc(size_t size)
{
	ASSERT(size > 0);

	void *ptr = NULL;

#ifdef _WINDOWS

	ptr = _aligned_malloc(size, 16);

#elif _ANDROID

	ptr = memalign(16, size);

#else

	ptr = malloc(size);

#endif

	return ptr;
}

void _free(void *ptr)
{
	ASSERT(ptr);

#ifdef _WINDOWS

	_aligned_free(ptr);

#else

	free(ptr);

#endif
}
