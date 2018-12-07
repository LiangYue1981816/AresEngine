#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rbtree.h"
#include "MemoryHeap.h"
#include "MemorySystem.h"


#ifndef ASSERT
#define ASSERT assert
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define ALIGN_BYTE(a, b) ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)   ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)  ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)  ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)  ALIGN_BYTE(a, 4096)


#define NODE_INDEX(size) (((size) / BLOCK_UNIT_SIZE) - 1)

static const uint32_t BLOCK_POOL_SIZE = 4 * 1024 * 1024;
static const uint32_t BLOCK_UNIT_SIZE = 4 * 1024;

struct BLOCK;
struct BLOCK_POOL;

struct BLOCK {
	BLOCK_POOL *pPool;

	BLOCK *pNext;
	BLOCK *pPrev;

	BLOCK *pFreeNext;
	BLOCK *pFreePrev;

	uint32_t dwSize;
	uint32_t dwInUse;

#ifdef __LP64__
	uint32_t dwReserve0;
	uint32_t dwReserve1;
	uint32_t dwReserve2;
#endif
	uint32_t dwAddress;
};

struct BLOCK_NODE {
	rb_node node;

	uint32_t dwSize;
	BLOCK *pBlockHead;
};

struct BLOCK_POOL {
	uint32_t dwSize;
	uint32_t dwFullSize;

	rb_root root;
	BLOCK_NODE *nodes;

	BLOCK_POOL *pNext;
	BLOCK_POOL *pPrev;
};

struct HEAP_ALLOCATOR {
	std::atomic_flag lock;
	BLOCK_POOL *pBlockPoolHead;
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


static void HEAP_InitNodes(BLOCK_POOL *pBlockPool, uint32_t dwNodeCount)
{
	pBlockPool->root = RB_ROOT;

	for (uint32_t indexNode = 0; indexNode < dwNodeCount; indexNode++) {
		pBlockPool->nodes[indexNode].dwSize = (indexNode + 1) * BLOCK_UNIT_SIZE;
		pBlockPool->nodes[indexNode].pBlockHead = nullptr;
	}
}

static BLOCK* HEAP_SearchBlock(BLOCK_POOL *pBlockPool, uint32_t dwSize)
{
	BLOCK_NODE *pBlockNode = nullptr;
	rb_node *node = pBlockPool->root.rb_node;

	while (node) {
		BLOCK_NODE *pBlockNodeCur = container_of(node, BLOCK_NODE, node);

		if (dwSize > pBlockNodeCur->dwSize) {
			node = node->rb_right;
			continue;
		}

		pBlockNode = pBlockNodeCur;

		if (dwSize < pBlockNodeCur->dwSize) {
			node = node->rb_left;
			continue;
		}

		ASSERT(pBlockNode->pBlockHead);
		ASSERT(pBlockNode->pBlockHead->dwInUse == false);
		ASSERT(pBlockNode->pBlockHead->dwSize / BLOCK_UNIT_SIZE * BLOCK_UNIT_SIZE >= dwSize);

		break;
	}

	return pBlockNode ? pBlockNode->pBlockHead : nullptr;
}

static void HEAP_InsertBlock(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
{
	ASSERT(pBlock->dwInUse == false);

	BLOCK_NODE *pBlockNode = &pBlockPool->nodes[NODE_INDEX(pBlock->dwSize)];
	rb_node **node = &(pBlockPool->root.rb_node);
	rb_node *parent = nullptr;

	while (*node) {
		BLOCK_NODE *pBlockNodeCur = container_of(*node, BLOCK_NODE, node);

		parent = *node;

		if (pBlockNode->dwSize > pBlockNodeCur->dwSize) {
			node = &(*node)->rb_right;
			continue;
		}

		if (pBlockNode->dwSize < pBlockNodeCur->dwSize) {
			node = &(*node)->rb_left;
			continue;
		}

		ASSERT(pBlockNode == pBlockNodeCur);

		pBlock->pFreePrev = nullptr;
		pBlock->pFreeNext = pBlockNode->pBlockHead;
		pBlockNode->pBlockHead->pFreePrev = pBlock;
		pBlockNode->pBlockHead = pBlock;

		return;
	}

	pBlock->pFreeNext = nullptr;
	pBlock->pFreePrev = nullptr;
	pBlockNode->pBlockHead = pBlock;

	rb_init_node(&pBlockNode->node);
	rb_link_node(&pBlockNode->node, parent, node);
	rb_insert_color(&pBlockNode->node, &pBlockPool->root);
}

static void HEAP_RemoveBlock(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
{
	ASSERT(pBlock->dwInUse == false);
	BLOCK_NODE *pBlockNode = &pBlockPool->nodes[NODE_INDEX(pBlock->dwSize)];

	if (pBlock->pFreeNext) {
		pBlock->pFreeNext->pFreePrev = pBlock->pFreePrev;
	}

	if (pBlock->pFreePrev) {
		pBlock->pFreePrev->pFreeNext = pBlock->pFreeNext;
	}

	if (pBlockNode->pBlockHead == pBlock) {
		pBlockNode->pBlockHead = pBlock->pFreeNext;

		if (pBlockNode->pBlockHead == nullptr) {
			rb_erase(&pBlockNode->node, &pBlockPool->root);
		}
	}
}

static BLOCK_POOL* HEAP_CreatePool(uint32_t dwMemSize)
{
	const uint32_t dwBlockPoolSize = ALIGN_16BYTE(max(dwMemSize, BLOCK_POOL_SIZE));
	const uint32_t dwNodeCount = dwBlockPoolSize / BLOCK_UNIT_SIZE;

	BLOCK_POOL *pBlockPool = (BLOCK_POOL *)_malloc(ALIGN_16BYTE(sizeof(BLOCK_POOL)) + ALIGN_16BYTE(sizeof(BLOCK)) + dwBlockPoolSize);
	{
		pBlockPool->dwSize = dwBlockPoolSize;
		pBlockPool->dwFullSize = dwBlockPoolSize;

		pBlockPool->pNext = nullptr;
		pBlockPool->pPrev = nullptr;

		pBlockPool->nodes = (BLOCK_NODE *)_malloc(sizeof(BLOCK_NODE) * dwNodeCount);
		{
			HEAP_InitNodes(pBlockPool, dwNodeCount);
		}

		BLOCK *pBlock = (BLOCK *)((uint8_t *)pBlockPool + ALIGN_16BYTE(sizeof(BLOCK_POOL)));
		{
			pBlock->pPool = pBlockPool;
			pBlock->pNext = nullptr;
			pBlock->pPrev = nullptr;
			pBlock->dwSize = dwBlockPoolSize;
			pBlock->dwInUse = false;
			HEAP_InsertBlock(pBlockPool, pBlock);
		}
	}

	return pBlockPool;
}

static void HEAP_DestroyPool(BLOCK_POOL *pBlockPool)
{
	_free(pBlockPool->nodes);
	_free(pBlockPool);
}

static void* HEAP_PoolAlloc(BLOCK_POOL *pBlockPool, uint32_t dwMemSize)
{
	uint32_t *pPointer = nullptr;

	if (pBlockPool->dwSize >= dwMemSize) {
		if (BLOCK *pBlock = HEAP_SearchBlock(pBlockPool, dwMemSize)) {
			HEAP_RemoveBlock(pBlockPool, pBlock);

			if (pBlock->dwSize >= dwMemSize + ALIGN_16BYTE(sizeof(BLOCK)) + BLOCK_UNIT_SIZE) {
				BLOCK *pBlockNext = (BLOCK *)((uint8_t *)pBlock + dwMemSize + ALIGN_16BYTE(sizeof(BLOCK)));
				{
					pBlockNext->pPool = pBlock->pPool;
					pBlockNext->dwSize = pBlock->dwSize - dwMemSize - ALIGN_16BYTE(sizeof(BLOCK));
					pBlockNext->dwInUse = false;

					pBlockNext->pNext = pBlock->pNext;
					pBlockNext->pPrev = pBlock;
					pBlock->pNext = pBlockNext;

					if (pBlockNext->pNext) {
						pBlockNext->pNext->pPrev = pBlockNext;
					}

					HEAP_InsertBlock(pBlockPool, pBlockNext);
				}

				pBlock->dwSize = dwMemSize;
			}

			pBlock->dwInUse = true;
			pBlockPool->dwSize -= pBlock->dwSize;

			pPointer = &pBlock->dwAddress; *pPointer++ = dwMemSize;
		}
	}

	return pPointer;
}

static BLOCK* HEAP_Merge(BLOCK_POOL *pBlockPool, BLOCK *pBlock, BLOCK *pBlockNext)
{
	ASSERT((uint8_t *)pBlock + pBlock->dwSize + ALIGN_16BYTE(sizeof(BLOCK)) == (uint8_t *)pBlockNext);

	pBlock->dwSize = pBlock->dwSize + pBlockNext->dwSize + ALIGN_16BYTE(sizeof(BLOCK));
	pBlock->pNext = pBlockNext->pNext;

	if (pBlockNext->pNext) {
		pBlockNext->pNext->pPrev = pBlock;
	}

	return pBlock;
}

static void HEAP_PoolFree(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
{
	pBlock->dwInUse = false;
	pBlockPool->dwSize += pBlock->dwSize;

	if (pBlock->pNext && pBlock->pNext->dwInUse == false) {
		HEAP_RemoveBlock(pBlockPool, pBlock->pNext);
		pBlock = HEAP_Merge(pBlockPool, pBlock, pBlock->pNext);
	}

	if (pBlock->pPrev && pBlock->pPrev->dwInUse == false) {
		HEAP_RemoveBlock(pBlockPool, pBlock->pPrev);
		pBlock = HEAP_Merge(pBlockPool, pBlock->pPrev, pBlock);
	}

	HEAP_InsertBlock(pBlockPool, pBlock);
}

HEAP_ALLOCATOR* HEAP_Create(void)
{
	HEAP_ALLOCATOR *pHeapAllocator = (HEAP_ALLOCATOR *)_malloc(sizeof(HEAP_ALLOCATOR));
	pHeapAllocator->pBlockPoolHead = nullptr;
	atomic_spin_init(&pHeapAllocator->lock);

	return pHeapAllocator;
}

void HEAP_Destroy(HEAP_ALLOCATOR *pHeapAllocator)
{
	if (BLOCK_POOL *pBlockPool = pHeapAllocator->pBlockPoolHead) {
		BLOCK_POOL *pBlockPoolNext = nullptr;
		do {
			pBlockPoolNext = pBlockPool->pNext;
			HEAP_DestroyPool(pBlockPool);
		} while ((pBlockPool = pBlockPoolNext) != nullptr);
	}

	_free(pHeapAllocator);
}

void* HEAP_Alloc(HEAP_ALLOCATOR *pHeapAllocator, size_t size)
{
	uint32_t *pPointer = nullptr;

	if (pHeapAllocator) {
		atomic_spin_lock(&pHeapAllocator->lock);
		{
			const uint32_t dwMemSize = (uint32_t)ALIGN_BYTE(size, BLOCK_UNIT_SIZE);

			do {
				if (BLOCK_POOL *pBlockPool = pHeapAllocator->pBlockPoolHead) {
					do {
						if ((pPointer = (uint32_t *)HEAP_PoolAlloc(pBlockPool, dwMemSize)) != nullptr) {
							goto RET;
						}
					} while ((pBlockPool = pBlockPool->pNext) != nullptr);
				}

				BLOCK_POOL *pBlockPool = HEAP_CreatePool(dwMemSize);

				if (pHeapAllocator->pBlockPoolHead) {
					pHeapAllocator->pBlockPoolHead->pPrev = pBlockPool;
					pBlockPool->pNext = pHeapAllocator->pBlockPoolHead;
				}

				pHeapAllocator->pBlockPoolHead = pBlockPool;
			} while (true);
		RET:
			;
		}
		atomic_spin_unlock(&pHeapAllocator->lock);
	}

	return pPointer;
}

bool HEAP_Free(HEAP_ALLOCATOR *pHeapAllocator, void *pPointer)
{
	if (pHeapAllocator) {
		atomic_spin_lock(&pHeapAllocator->lock);
		{
			BLOCK *pBlock = (BLOCK *)((uint8_t *)pPointer - ALIGN_16BYTE(sizeof(BLOCK)));
			BLOCK_POOL *pBlockPool = pBlock->pPool;

			HEAP_PoolFree(pBlockPool, pBlock);

			if (pBlockPool->dwSize == pBlockPool->dwFullSize) {
				if (pHeapAllocator->pBlockPoolHead == pBlockPool) {
					pHeapAllocator->pBlockPoolHead =  pBlockPool->pNext;
				}

				if (pBlockPool->pPrev) {
					pBlockPool->pPrev->pNext = pBlockPool->pNext;
				}

				if (pBlockPool->pNext) {
					pBlockPool->pNext->pPrev = pBlockPool->pPrev;
				}

				HEAP_DestroyPool(pBlockPool);
			}
		}
		atomic_spin_unlock(&pHeapAllocator->lock);
		return true;
	}

	return false;
}
