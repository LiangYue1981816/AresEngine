#pragma once


extern void InitAllocator(void);
extern void ExitAllocator(void);
extern void* AllocMemory(size_t size);
extern void FreeMemory(void *pPointer);
