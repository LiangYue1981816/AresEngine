#include <stdlib.h>
#include <stdint.h>
#include "pthread.h"
#include "MemoryPool.h"
#include "MemoryHeap.h"
#include "MemorySystem.h"


#define ALIGN_BYTE(a, b) ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)   ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)  ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)  ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)  ALIGN_BYTE(a, 4096)


// ABBBBBBCCCCCCCCCCCCCCCCCCCCCCCCC
// A: system memory or allocator memory
// B: thread id. Max Thread Number 64
// C: size. Max Size 32MB
#define GET_MEM_SIZE(ptr) (*((uint32_t *)(ptr) - 1) & 0x01FFFFFF)

#define GET_MEM_SYSTEM(ptr) (*((uint32_t *)(ptr) - 1) & 0x80000000)
#define SET_MEM_SYSTEM(ptr)  *((uint32_t *)(ptr) - 1) = (*((uint32_t *)(ptr) - 1) | 0x80000000)

#define GET_MEM_THREAD_ID(ptr) ((*((uint32_t *)(ptr) - 1) & 0x7E000000) >> 25)
#define SET_MEM_THREAD_ID(ptr)   *((uint32_t *)(ptr) - 1) = ((indexThread & 0x0000003F) << 25) | (*((uint32_t *)(ptr) - 1) & 0x81FFFFFF)


#define MAX_THREAD_COUNT 64
static bool bInitAllocator = false;
static pthread_t threads[MAX_THREAD_COUNT] = { NULL };
static POOL_ALLOCATOR *pPoolAllocators[MAX_THREAD_COUNT] = { NULL };
static HEAP_ALLOCATOR *pHeapAllocator = NULL;


static int GetThreadIndex(void)
{
	int indexThread = -1;
	pthread_t thread = pthread_self();

	for (int index = 0; index < MAX_THREAD_COUNT; index++) {
#ifdef _WINDOWS
		if (threads[index].p == thread.p) {
			indexThread = index;
			break;
		}

		if (threads[index].p == NULL) {
			threads[index].p = thread.p;
			indexThread = index;
			break;
		}
#else
		if (threads[index] == thread) {
			indexThread = index;
			break;
		}

		if (threads[index] == NULL) {
			threads[index] = thread;
			indexThread = index;
			break;
		}
#endif
	}

	return indexThread;
}

void InitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator == false) {
		pHeapAllocator = HEAP_Create();

		for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
			pPoolAllocators[indexThread] = POOL_Create(pHeapAllocator);
		}

		bInitAllocator = true;
	}
#endif
}

void ExitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator) {
		bInitAllocator = false;

		for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
			POOL_Destroy(pHeapAllocator, pPoolAllocators[indexThread]);
		}

		HEAP_Destroy(pHeapAllocator);
	}
#endif
}

void* AllocMemory(size_t size)
{
	size = ALIGN_4BYTE(size);

#ifdef MEMORY_ALLOCATOR
	uint32_t *pPointer = NULL;
	int indexThread = GetThreadIndex();

	if (bInitAllocator && indexThread >= 0 && size < 0x01FFFFFF) {
		if (pPointer == NULL) {
			pPointer = (uint32_t *)POOL_Alloc(pHeapAllocator, pPoolAllocators[indexThread], size);
		}

		if (pPointer == NULL) {
			pPointer = (uint32_t *)HEAP_Alloc(pHeapAllocator, size);
		}

		SET_MEM_THREAD_ID(pPointer);
	}
	else {
		pPointer = (uint32_t *)_malloc(size + 4); *pPointer++ = (uint32_t)size;
		SET_MEM_SYSTEM(pPointer);
	}

	return pPointer;
#else
	return _malloc(size);
#endif
}

void FreeMemory(void *pPointer)
{
	if (pPointer == NULL) {
		return;
	}

#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator && GET_MEM_SYSTEM(pPointer) == 0) {
		uint32_t indexThread = GET_MEM_THREAD_ID(pPointer);

		if (pPointer) {
			if (POOL_Free(pHeapAllocator, pPoolAllocators[indexThread], pPointer)) {
				pPointer = NULL;
			}
		}

		if (pPointer) {
			if (HEAP_Free(pHeapAllocator, pPointer)) {
				pPointer = NULL;
			}
		}
	}
	else {
		_free((uint8_t *)pPointer - 4);
	}
#else
	_free(pPointer);
#endif
}
