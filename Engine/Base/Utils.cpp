#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "windows.h"


unsigned int tick(void)
{
#ifdef _WIN32

	LARGE_INTEGER freq;
	LARGE_INTEGER count;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&count);

	return (unsigned int)(((double)count.QuadPart / freq.QuadPart) * 1000000);

#else

	struct timeval curtime;
	gettimeofday(&curtime, 0);
	return (unsigned int)(curtime.tv_sec * 1000000 + curtime.tv_usec);

#endif
}

unsigned int HashValue(const char *szString)
{
	const char *c = szString;
	unsigned int dwHashValue = 0x00000000;

	while (*c) {
//		dwHashValue = (dwHashValue << 5) - dwHashValue + (*c == '/' ? '\\' : *c); c++;
		dwHashValue = (dwHashValue << 4) - dwHashValue + (*c == '/' ? '\\' : *c); c++;
	}

	return dwHashValue ? dwHashValue : 0xffffffff;
}

void LogOutput(const char *szTag, const char *szFormat, ...)
{
	static char szText[128 * 1024];

	va_list vaList;
	va_start(vaList, szFormat);
	vsprintf(szText, szFormat, vaList);
	va_end(vaList);

#ifdef _WIN32

	OutputDebugString(szText);

#endif

	printf(szText);
}
