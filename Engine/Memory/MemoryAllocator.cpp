#include <stdlib.h>
#include <stdint.h>
#include "pthread.h"
#include "MemoryPool.h"
#include "MemoryHeap.h"


#define GET_MEM_SIZE(ptr) (*((uint32_t *)(ptr) - 1) & 0x00ffffff)
#define GET_MEM_THREAD_ID(ptr) ((*((uint32_t *)(ptr) - 1) & 0xff000000) >> 24)
#define SET_MEM_THREAD_ID(ptr, id) *((uint32_t *)(ptr) - 1) = (indexThread << 24) | (*((uint32_t *)(ptr) - 1) & 0x00ffffff);


static pthread_t threads[THREAD_COUNT] = { NULL };
static pthread_spinlock_t locks[THREAD_COUNT] = { NULL };
static HEAP_ALLOCATOR *pHeapAllocators[THREAD_COUNT] = { NULL };
static POOL_ALLOCATOR *pPoolAllocators[THREAD_COUNT] = { NULL };


static int GetThreadIndex(void)
{
	int indexThread = 0;
	pthread_t thread = pthread_self();

	for (int index = 0; index < THREAD_COUNT; index++) {
		if (threads[index].p == thread.p) {
			indexThread = index;
			break;
		}

		if (threads[index].p == NULL) {
			threads[index].p = thread.p;
			indexThread = index;
			break;
		}
	}

	return indexThread;
}

void InitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		pthread_spin_init(&locks[indexThread], PTHREAD_PROCESS_PRIVATE);
		pHeapAllocators[indexThread] = HEAP_Create();
		pPoolAllocators[indexThread] = POOL_Create(pHeapAllocators[indexThread]);
	}
#endif
}

void ExitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		POOL_Destroy(pHeapAllocators[indexThread], pPoolAllocators[indexThread]);
		HEAP_Destroy(pHeapAllocators[indexThread]);
		pthread_spin_destroy(&locks[indexThread]);
	}
#endif
}

void* AllocMemory(size_t size)
{
#ifdef MEMORY_ALLOCATOR
	void *pPointer = NULL;
	int indexThread = GetThreadIndex();

	if (pPointer == NULL) {
		pthread_spin_lock(&locks[indexThread]);
		{
			pPointer = POOL_Alloc(pHeapAllocators[indexThread], pPoolAllocators[indexThread], size);
		}
		pthread_spin_unlock(&locks[indexThread]);
	}

	if (pPointer == NULL) {
		pthread_spin_lock(&locks[indexThread]);
		{
			pPointer = HEAP_Alloc(pHeapAllocators[indexThread], size);
		}
		pthread_spin_unlock(&locks[indexThread]);
	}

	if (pPointer) {
		SET_MEM_THREAD_ID(pPointer, indexThread);
	}

	return pPointer;
#else
	return _malloc(size);
#endif
}

void FreeMemory(void *pPointer)
{
#ifdef MEMORY_ALLOCATOR
	bool bFree = false;
	int indexThread = GET_MEM_THREAD_ID(pPointer);

	if (bFree == false) {
		pthread_spin_lock(&locks[indexThread]);
		{
			bFree = POOL_Free(pHeapAllocators[indexThread], pPoolAllocators[indexThread], pPointer);
		}
		pthread_spin_unlock(&locks[indexThread]);
	}

	if (bFree == false) {
		pthread_spin_lock(&locks[indexThread]);
		{
			bFree = HEAP_Free(pHeapAllocators[indexThread], pPointer);
		}
		pthread_spin_unlock(&locks[indexThread]);
	}
#else
	_free(pPointer);
#endif
}
