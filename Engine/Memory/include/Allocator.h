#pragma once
#include "PreHeader.h"


#define GET_MEM_SIZE(ptr) ((ptr) != nullptr ? (*((uint32_t*)(ptr) - 1) & 0x01FFFFFF) : 0)


CALL_API void InitAllocator(void);
CALL_API void ExitAllocator(void);
CALL_API void* AllocMemory(size_t size);
CALL_API void FreeMemory(void* pPointer);
