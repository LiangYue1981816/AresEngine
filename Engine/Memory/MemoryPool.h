#pragma once


struct HEAP_ALLOCATOR;
struct POOL_ALLOCATOR;
extern POOL_ALLOCATOR* POOL_Create(HEAP_ALLOCATOR *pHeapAllocator);
extern void POOL_Destroy(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator);
extern void* POOL_Alloc(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, size_t size);
extern bool POOL_Free(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, void *pPointer);
