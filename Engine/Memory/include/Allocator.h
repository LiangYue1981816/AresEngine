#pragma once
#include "Define.h"


CALL_API void InitAllocator(void);
CALL_API void ExitAllocator(void);
CALL_API void* AllocMemory(size_t size);
CALL_API void FreeMemory(void *pPointer);
