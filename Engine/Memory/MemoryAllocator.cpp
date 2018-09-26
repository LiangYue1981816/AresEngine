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
// B: thread id
// C: size

#define GET_MEM_SIZE(ptr) (*((uint32_t *)(ptr) - 1) & 0x01FFFFFF)

#define GET_MEM_SYSTEM(ptr) (*((uint32_t *)(ptr) - 1) & 0x80000000)
#define SET_MEM_SYSTEM(ptr)  *((uint32_t *)(ptr) - 1) = (*((uint32_t *)(ptr) - 1) | 0x80000000)

#define GET_MEM_THREAD_ID(ptr) ((*((uint32_t *)(ptr) - 1) & 0x7E000000) >> 25)
#define SET_MEM_THREAD_ID(ptr)   *((uint32_t *)(ptr) - 1) = ((indexThread & 0x0000003F) << 25) | (*((uint32_t *)(ptr) - 1) & 0x81FFFFFF)


static bool bInitAllocator = false;
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
	if (bInitAllocator == false) {
		bInitAllocator = true;

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			pthread_spin_init(&locks[indexThread], PTHREAD_PROCESS_PRIVATE);
			pHeapAllocators[indexThread] = HEAP_Create();
			pPoolAllocators[indexThread] = POOL_Create(pHeapAllocators[indexThread]);
		}
	}
#endif
}

void ExitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator) {
		bInitAllocator = false;

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			POOL_Destroy(pHeapAllocators[indexThread], pPoolAllocators[indexThread]);
			HEAP_Destroy(pHeapAllocators[indexThread]);
			pthread_spin_destroy(&locks[indexThread]);
		}
	}
#endif
}

void* AllocMemory(size_t size)
{
	size = ALIGN_4BYTE(size);

#ifdef MEMORY_ALLOCATOR
	uint32_t *pPointer = NULL;

	if (bInitAllocator && size < 0x01FFFFFF) {
		int indexThread = GetThreadIndex();

		if (pPointer == NULL) {
			pthread_spin_lock(&locks[indexThread]);
			{
				pPointer = (uint32_t *)POOL_Alloc(pHeapAllocators[indexThread], pPoolAllocators[indexThread], size);
			}
			pthread_spin_unlock(&locks[indexThread]);
		}

		if (pPointer == NULL) {
			pthread_spin_lock(&locks[indexThread]);
			{
				pPointer = (uint32_t *)HEAP_Alloc(pHeapAllocators[indexThread], size);
			}
			pthread_spin_unlock(&locks[indexThread]);
		}

		SET_MEM_THREAD_ID(pPointer);
	}
	else {
		pPointer = (uint32_t *)_malloc(size + 4); *pPointer++ = size;
		SET_MEM_SYSTEM(pPointer);
	}

	return pPointer;
#else
	return _malloc(size);
#endif
}

void FreeMemory(void *pPointer)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator && GET_MEM_SYSTEM(pPointer) == 0) {
		uint32_t indexThread = GET_MEM_THREAD_ID(pPointer);

		if (pPointer) {
			pthread_spin_lock(&locks[indexThread]);
			{
				if (POOL_Free(pHeapAllocators[indexThread], pPoolAllocators[indexThread], pPointer)) {
					pPointer = NULL;
				}
			}
			pthread_spin_unlock(&locks[indexThread]);
		}

		if (pPointer) {
			pthread_spin_lock(&locks[indexThread]);
			{
				if (HEAP_Free(pHeapAllocators[indexThread], pPointer)) {
					pPointer = NULL;
				}
			}
			pthread_spin_unlock(&locks[indexThread]);
		}
	}
	else {
		_free((uint8_t *)pPointer - 4);
	}
#else
	_free(pPointer);
#endif
}


class CMemoryAllocator
{
public:
	CMemoryAllocator(void)
	{
		InitAllocator();
	}
	~CMemoryAllocator(void)
	{
		ExitAllocator();
	}
};

static CMemoryAllocator allocator;
