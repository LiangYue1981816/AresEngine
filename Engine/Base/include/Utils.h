#pragma once
#include "PreHeader.h"


CALL_API unsigned int NumCpuCores(void);
CALL_API unsigned int Tick(void);
CALL_API unsigned int HashValue(const char* szString);
CALL_API unsigned int HashValueFormat(const char* szFormat, ...);
CALL_API unsigned int HashValue(const unsigned char* pBuffer, int length, int stride = 1);

#define LOG_INFO  0
#define LOG_WARN  1
#define LOG_ERROR 2
CALL_API void LogOutput(int prio, const char* szTag, const char* szFormat, ...);

CALL_API void splitfilename(const char* name, char* fname, char* ext);
CALL_API int fexist(const char* name);
CALL_API size_t fsize(FILE* stream);
CALL_API size_t freadline(char* buffer, size_t size, FILE* stream);
CALL_API size_t freadstring(char* buffer, size_t size, FILE* stream);
CALL_API size_t fwritestring(const char* buffer, size_t size, FILE* stream);

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
CALL_API int stricmp(const char* src, const char* dst);
CALL_API int strnicmp(const char* src, const char* dst, int count);

#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_SUBDIR 0x10
#define _A_ARCH   0x20

struct _finddata_t {
	char name[_MAX_STRING];
	unsigned int attrib;
};

CALL_API long _findfirst(const char* pattern, struct _finddata_t* data);
CALL_API long _findnext(long id, struct _finddata_t* data);
CALL_API long _findclose(long id);
#endif
