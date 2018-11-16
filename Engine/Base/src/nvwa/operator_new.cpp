#undef  _DEBUG_NEW_REDEFINE_NEW
#define _DEBUG_NEW_REDEFINE_NEW 0
#include "debug_new.h"


#if defined (_DEBUG) || defined (DEBUG)
void* operator new (size_t size, const char* file, int line) noexcept
{
	return alloc_mem(size, file, line, false);
}

void* operator new (size_t size)
{
	return alloc_mem(size, nullptr, 0, false);
}

void* operator new (size_t size, const std::nothrow_t&) noexcept
{
	return alloc_mem(size, nullptr, 0, false);
}

void* operator new[] (size_t size, const char* file, int line) noexcept
{
	return alloc_mem(size, file, line, true);
}

void* operator new[] (size_t size)
{
	return alloc_mem(size, nullptr, 0, true);
}

void* operator new[] (size_t size, const std::nothrow_t&) noexcept
{
	return alloc_mem(size, nullptr, 0, true);
}

void operator delete (void* ptr, const char* file, int line) noexcept
{
	free_pointer(ptr, nullptr, false);
}

void operator delete (void* ptr) noexcept
{
	free_pointer(ptr, nullptr, false);
}

void operator delete (void* ptr, const std::nothrow_t&) noexcept
{
	free_pointer(ptr, nullptr, false);
}

void operator delete[] (void* ptr, const char* file, int line) noexcept
{
	free_pointer(ptr, nullptr, true);
}

void operator delete[] (void* ptr) noexcept
{
	free_pointer(ptr, nullptr, true);
}

void operator delete[] (void* ptr, const std::nothrow_t&) noexcept
{
	free_pointer(ptr, nullptr, true);
}
#else
void* operator new (size_t size)
{
	return AllocMemory(size);
}

void* operator new (size_t size, const std::nothrow_t&) noexcept
{
	return AllocMemory(size);
}

void* operator new[] (size_t size)
{
	return AllocMemory(size);
}

void* operator new[] (size_t size, const std::nothrow_t&) noexcept
{
	return AllocMemory(size);
}

void operator delete (void* ptr) noexcept
{
	FreeMemory(ptr);
}

void operator delete (void* ptr, const std::nothrow_t&) noexcept
{
	FreeMemory(ptr);
}

void operator delete[] (void* ptr) noexcept
{
	FreeMemory(ptr);
}

void operator delete[] (void* ptr, const std::nothrow_t&) noexcept
{
	FreeMemory(ptr);
}
#endif
