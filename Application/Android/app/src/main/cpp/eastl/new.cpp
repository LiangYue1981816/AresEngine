#include "stdlib.h"


extern void* AllocMemory(size_t size);
extern void Free(void *pPointer);
extern void* alloc_mem(size_t size, const char* file, int line, bool is_array);
extern void free_pointer(void* usr_ptr, void* addr, bool is_array);


void* operator new[] (size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
#if defined (_DEBUG) || defined (DEBUG)
	return alloc_mem(size, file, line, true);
#else
	return AllocMemory(size);
#endif
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
#if defined (_DEBUG) || defined (DEBUG)
	return alloc_mem(size, file, line, true);
#else
	return AllocMemory(size);
#endif
}
