#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "Allocator.h"
#include "MemoryPool.h"
#include "MemoryHeap.h"
#include "MemorySystem.h"


#define ALIGN_BYTE(a, b) ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)   ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)  ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)  ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)  ALIGN_BYTE(a, 4096)


// ABBBBBBCCCCCCCCCCCCCCCCCCCCCCCCC
// A: system memory or allocator memory.
// B: pool id. max pool number 64.
// C: size. max size 32MB.
#define GET_MEM_SIZE(ptr) (*((uint32_t *)(ptr) - 1) & 0x01FFFFFF)

#define GET_MEM_SYSTEM(ptr) (*((uint32_t *)(ptr) - 1) & 0x80000000)
#define SET_MEM_SYSTEM(ptr)  *((uint32_t *)(ptr) - 1) = (*((uint32_t *)(ptr) - 1) | 0x80000000)

#define GET_MEM_POOL_ID(ptr) ((*((uint32_t *)(ptr) - 1) & 0x7E000000) >> 25)
#define SET_MEM_POOL_ID(ptr)   *((uint32_t *)(ptr) - 1) = ((indexPool & 0x0000003F) << 25) | (*((uint32_t *)(ptr) - 1) & 0x81FFFFFF)


#define MAX_POOL_COUNT 64
static bool bInitAllocator = false;
static std::thread::id threads[MAX_POOL_COUNT];
static POOL_ALLOCATOR *pPoolAllocators[MAX_POOL_COUNT] = { nullptr };
static HEAP_ALLOCATOR *pHeapAllocator = nullptr;


static int GetPoolIndex(void)
{
	std::thread::id thread = std::this_thread::get_id();

	for (int index = 0; index < MAX_POOL_COUNT; index++) {
		if (threads[index] == thread) {
			return index;
		}

		if (threads[index] == std::thread::id()) {
			threads[index] = thread;
			return index;
		}
	}

	return -1;
}

CALL_API void InitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator == false) {
		pHeapAllocator = HEAP_Create();

		for (int indexPool = 0; indexPool < MAX_POOL_COUNT; indexPool++) {
			pPoolAllocators[indexPool] = POOL_Create(pHeapAllocator);
		}

		bInitAllocator = true;
	}
#endif
}

CALL_API void ExitAllocator(void)
{
#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator) {
		bInitAllocator = false;

		for (int indexPool = 0; indexPool < MAX_POOL_COUNT; indexPool++) {
			POOL_Destroy(pHeapAllocator, pPoolAllocators[indexPool]);
		}

		HEAP_Destroy(pHeapAllocator);
	}
#endif
}

CALL_API void* AllocMemory(size_t size)
{
#ifdef MEMORY_ALLOCATOR
	uint32_t *pPointer = nullptr;

	if (bInitAllocator && size < 0x01FFFFFF) {
		int indexPool = GetPoolIndex();

		if (pPointer == nullptr) {
			pPointer = (uint32_t *)POOL_Alloc(pHeapAllocator, pPoolAllocators[indexPool], size);
		}

		if (pPointer == nullptr) {
			pPointer = (uint32_t *)HEAP_Alloc(pHeapAllocator, size);
		}

		SET_MEM_POOL_ID(pPointer);
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

CALL_API void FreeMemory(void *pPointer)
{
	if (pPointer == nullptr) {
		return;
	}

#ifdef MEMORY_ALLOCATOR
	if (bInitAllocator && GET_MEM_SYSTEM(pPointer) == 0) {
		int indexPool = GET_MEM_POOL_ID(pPointer);

		if (pPointer) {
			if (POOL_Free(pHeapAllocator, pPoolAllocators[indexPool], pPointer)) {
				pPointer = nullptr;
			}
		}

		if (pPointer) {
			if (HEAP_Free(pHeapAllocator, pPointer)) {
				pPointer = nullptr;
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
