#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pthread.h"
#include "MemoryPool.h"
#include "MemoryHeap.h"


#define ALIGN_BYTE(a, b) ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)   ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)  ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)  ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)  ALIGN_BYTE(a, 4096)


#define GET_MEM_SIZE(ptr) (*((uint32_t *)(ptr) - 1) & 0x01FFFFFF)

#define GET_BLOCK(pPointer) (BLOCK *)((uint8_t *)pPointer - ALIGN_4BYTE(sizeof(BLOCK)))
#define GET_BLOCK_POOL(pBlock) (BLOCK_POOL *)((uint8_t *)(pBlock) - (pBlock)->dwOffset - ALIGN_16BYTE(sizeof(BLOCK_POOL)))
#define GET_BLOCK_NEXT(pBlock) (BLOCK *)((uint8_t *)GET_BLOCK_POOL(pBlock) + (pBlock)->dwOffsetNext + ALIGN_16BYTE(sizeof(BLOCK_POOL)))

static const uint32_t BLOCK_POOL_COUNT = 4 * 1024 / 16;
static const uint32_t BLOCK_POOL_SIZE = 32 * 1024;

struct BLOCK;
struct BLOCK_POOL;

struct BLOCK {
	uint16_t dwOffset;
	uint16_t dwOffsetNext;
	uint32_t dwAddress;
};

struct BLOCK_POOL {
	BLOCK *pBlockHead;

	BLOCK_POOL *pNext;
	BLOCK_POOL *pPrev;

	BLOCK_POOL *pFreeNext;
	BLOCK_POOL *pFreePrev;

	uint32_t dwBlockIndex;
	uint32_t dwBlockCount;
};

struct BLOCK_POOL_HEAD {
	BLOCK_POOL *pBlockPoolHead;
	BLOCK_POOL *pBlockPoolFreeHead;
};

struct POOL_ALLOCATOR {
	std::atomic_flag lock;
	BLOCK_POOL_HEAD pools[BLOCK_POOL_COUNT];
};


static void atomic_spin_init(std::atomic_flag *flag)
{
	flag->clear();
}

static void atomic_spin_lock(std::atomic_flag *flag)
{
	while (flag->test_and_set(std::memory_order_acquire));
}

static void atomic_spin_unlock(std::atomic_flag *flag)
{
	flag->clear(std::memory_order_release);
}


static BLOCK_POOL* POOL_CreatePool(HEAP_ALLOCATOR *pHeapAllocator, uint32_t dwMemSize)
{
	const uint32_t dwBlockSize = ALIGN_16BYTE(sizeof(BLOCK)) + ALIGN_16BYTE(dwMemSize);
	const uint32_t dwBlockCount = (BLOCK_POOL_SIZE - ALIGN_16BYTE(sizeof(BLOCK_POOL))) / ALIGN_16BYTE(dwBlockSize);

	BLOCK_POOL *pBlockPool = (BLOCK_POOL *)HEAP_Alloc(pHeapAllocator, BLOCK_POOL_SIZE);
	{
		pBlockPool->pBlockHead = (BLOCK *)((uint8_t *)pBlockPool + ALIGN_16BYTE(sizeof(BLOCK_POOL)));
		pBlockPool->pNext = nullptr;
		pBlockPool->pPrev = nullptr;
		pBlockPool->pFreeNext = nullptr;
		pBlockPool->pFreePrev = nullptr;
		pBlockPool->dwBlockIndex = 0;
		pBlockPool->dwBlockCount = dwBlockCount;
	}

	BLOCK *pBlock = pBlockPool->pBlockHead;
	{
		for (uint32_t indexBlock = 0; indexBlock < dwBlockCount; indexBlock++) {
			pBlock->dwOffset = indexBlock * dwBlockSize;
			pBlock->dwOffsetNext = pBlock->dwOffset + dwBlockSize;
			pBlock = GET_BLOCK_NEXT(pBlock);
		}
	}

	return pBlockPool;
}

static void POOL_DestroyPool(HEAP_ALLOCATOR *pHeapAllocator, BLOCK_POOL *pBlockPool)
{
	HEAP_Free(pHeapAllocator, pBlockPool);
}

POOL_ALLOCATOR* POOL_Create(HEAP_ALLOCATOR *pHeapAllocator)
{
	POOL_ALLOCATOR *pPoolAllocator = (POOL_ALLOCATOR *)HEAP_Alloc(pHeapAllocator, sizeof(POOL_ALLOCATOR));

	for (uint32_t indexPool = 0; indexPool < BLOCK_POOL_COUNT; indexPool++) {
		pPoolAllocator->pools[indexPool].pBlockPoolHead = nullptr;
		pPoolAllocator->pools[indexPool].pBlockPoolFreeHead = nullptr;
	}
	atomic_spin_init(&pPoolAllocator->lock);

	return pPoolAllocator;
}

void POOL_Destroy(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator)
{
	for (uint32_t indexPool = 0; indexPool < BLOCK_POOL_COUNT; indexPool++) {
		if (BLOCK_POOL *pBlockPool = pPoolAllocator->pools[indexPool].pBlockPoolHead) {
			BLOCK_POOL *pBlockPoolNext = nullptr;

			do {
				pBlockPoolNext = pBlockPool->pNext;
				POOL_DestroyPool(pHeapAllocator, pBlockPool);
			} while ((pBlockPool = pBlockPoolNext) != nullptr);
		}
	}

	HEAP_Free(pHeapAllocator, pPoolAllocator);
}

void* POOL_Alloc(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, size_t size)
{
	uint32_t *pPointer = nullptr;

	if (pHeapAllocator && pPoolAllocator) {
		const uint32_t dwMemSize = (uint32_t)ALIGN_16BYTE(size);
		const uint32_t dwIndexPool = dwMemSize / 16;

		if (dwIndexPool < BLOCK_POOL_COUNT) {
			atomic_spin_lock(&pPoolAllocator->lock);
			{
				BLOCK_POOL_HEAD *pPoolHead = &pPoolAllocator->pools[dwIndexPool];

				if (pPoolHead->pBlockPoolFreeHead == nullptr) {
					pPoolHead->pBlockPoolFreeHead = POOL_CreatePool(pHeapAllocator, dwMemSize);

					if (pPoolHead->pBlockPoolHead) {
						pPoolHead->pBlockPoolHead->pPrev = pPoolHead->pBlockPoolFreeHead;
						pPoolHead->pBlockPoolFreeHead->pNext = pPoolHead->pBlockPoolHead;
					}

					pPoolHead->pBlockPoolHead = pPoolHead->pBlockPoolFreeHead;
				}

				pPointer = &pPoolHead->pBlockPoolFreeHead->pBlockHead->dwAddress;
				pPoolHead->pBlockPoolFreeHead->pBlockHead = GET_BLOCK_NEXT(pPoolHead->pBlockPoolFreeHead->pBlockHead);
				pPoolHead->pBlockPoolFreeHead->dwBlockIndex++;

				if (pPoolHead->pBlockPoolFreeHead->dwBlockIndex == pPoolHead->pBlockPoolFreeHead->dwBlockCount) {
					pPoolHead->pBlockPoolFreeHead = pPoolHead->pBlockPoolFreeHead->pFreeNext;
				}

				*pPointer++ = dwMemSize;
			}
			atomic_spin_unlock(&pPoolAllocator->lock);
		}
	}

	return pPointer;
}

bool POOL_Free(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, void *pPointer)
{
	if (pHeapAllocator && pPoolAllocator) {
		const uint32_t dwMemSize = GET_MEM_SIZE(pPointer);
		const uint32_t dwIndexPool = dwMemSize / 16;

		if (dwIndexPool < BLOCK_POOL_COUNT) {
			atomic_spin_lock(&pPoolAllocator->lock);
			{
				BLOCK *pBlock = GET_BLOCK(pPointer);
				BLOCK_POOL *pBlockPool = GET_BLOCK_POOL(pBlock);
				BLOCK_POOL_HEAD *pPoolHead = &pPoolAllocator->pools[dwIndexPool];

				if (pBlockPool->dwBlockIndex == pBlockPool->dwBlockCount) {
					pBlockPool->pFreeNext = nullptr;
					pBlockPool->pFreePrev = nullptr;

					if (pPoolHead->pBlockPoolFreeHead) {
						pPoolHead->pBlockPoolFreeHead->pFreePrev = pBlockPool;
						pBlockPool->pFreeNext = pPoolHead->pBlockPoolFreeHead;
					}

					pPoolHead->pBlockPoolFreeHead = pBlockPool;
				}
				else {
					pBlock->dwOffsetNext = pBlockPool->pBlockHead->dwOffset;
				}

				pBlockPool->dwBlockIndex--;

				if (pBlockPool->dwBlockIndex == 0) {
					if (pBlockPool->pPrev) pBlockPool->pPrev->pNext = pBlockPool->pNext;
					if (pBlockPool->pNext) pBlockPool->pNext->pPrev = pBlockPool->pPrev;
					if (pPoolHead->pBlockPoolHead == pBlockPool) pPoolHead->pBlockPoolHead = pBlockPool->pNext;

					if (pBlockPool->pFreePrev) pBlockPool->pFreePrev->pFreeNext = pBlockPool->pFreeNext;
					if (pBlockPool->pFreeNext) pBlockPool->pFreeNext->pFreePrev = pBlockPool->pFreePrev;
					if (pPoolHead->pBlockPoolFreeHead == pBlockPool) pPoolHead->pBlockPoolFreeHead = pBlockPool->pFreeNext;

					POOL_DestroyPool(pHeapAllocator, pBlockPool);
				}
				else {
					pBlockPool->pBlockHead = pBlock;
				}
			}
			atomic_spin_unlock(&pPoolAllocator->lock);
			return true;
		}
	}

	return false;
}
