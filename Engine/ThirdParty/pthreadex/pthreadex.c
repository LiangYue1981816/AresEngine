#include "pthreadex.h"


#ifdef PLATFORM_WINDOWS
#include <windows.h>
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;     // Must be 0x1000.
	LPCSTR szName;    // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

static void SetThreadName(DWORD dwThreadID, const char* szName)
{
	#define MS_VC_EXCEPTION 0x406D1388

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
}
#endif

void pthread_set_name(pthread_t thread, const char *name)
{
#ifdef PLATFORM_WINDOWS
	unsigned int dwWin32ThreadID;
	if (dwWin32ThreadID = pthread_getw32threadid_np(thread)) {
		SetThreadName(dwWin32ThreadID, name);
	}
#elif PLATFORM_ANDROID
	pthread_setname_np(thread, name);
#endif
}
