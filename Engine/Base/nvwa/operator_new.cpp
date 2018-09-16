#undef  _DEBUG_NEW_REDEFINE_NEW
#define _DEBUG_NEW_REDEFINE_NEW 0
#include "debug_new.h"


#ifdef _DEBUG
void* operator new(size_t size, const char* file, int line)
{
	return alloc_mem(size, file, line, false);
}

void* operator new(size_t size)
{
	return alloc_mem(size, NULL, 0, false);
}

void* operator new(size_t size, const std::nothrow_t&) noexcept
{
	return alloc_mem(size, NULL, 0, false);
}

void* operator new[](size_t size, const char* file, int line)
{
	return alloc_mem(size, file, line, true);
}

void* operator new[](size_t size)
{
	return alloc_mem(size, NULL, 0, true);
}

void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
	return alloc_mem(size, NULL, 0, true);
}

void operator delete(void* ptr, const char* file, int line)
{
	free_pointer(ptr, NULL, false);
}

void operator delete(void* ptr)
{
	free_pointer(ptr, NULL, false);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
	free_pointer(ptr, NULL, false);
}

void operator delete[](void* ptr, const char* file, int line)
{
	free_pointer(ptr, NULL, true);
}

void operator delete[](void* ptr)
{
	free_pointer(ptr, NULL, true);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
	free_pointer(ptr, NULL, true);
}
#else
void* operator new(size_t size)
{
	return malloc(size);
}

void* operator new(size_t size, const std::nothrow_t&) noexcept
{
	return malloc(size);
}

void* operator new[](size_t size)
{
	return malloc(size);
}

void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
	return malloc(size);
}

void operator delete(void* ptr)
{
	free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
	free(ptr);
}

void operator delete[](void* ptr)
{
	free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
	free(ptr);
}
#endif
