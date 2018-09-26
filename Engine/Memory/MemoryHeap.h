#pragma once


struct HEAP_ALLOCATOR;
extern HEAP_ALLOCATOR* HEAP_Create(void);
extern void HEAP_Destroy(HEAP_ALLOCATOR *pHeapAllocator);
extern void* HEAP_Alloc(HEAP_ALLOCATOR *pHeapAllocator, size_t size);
extern bool HEAP_Free(HEAP_ALLOCATOR *pHeapAllocator, void *pPointer);
