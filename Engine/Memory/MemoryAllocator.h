#pragma once


extern void InitAllocator(void);
extern void ExitAllocator(void);
extern void* AllocMemory(size_t size);
extern void FreeMemory(void *pPointer);

extern void* alloc_mem(size_t size, const char* file, int line, bool is_array);
extern void free_pointer(void* usr_ptr, void* addr, bool is_array);
extern int dump_memory_objects();
extern int check_mem_corruption();
