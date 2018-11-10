#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"


extern unsigned int tick(void);
extern unsigned int HashValue(const char *szString);
extern unsigned int HashValue(const unsigned char *pBuffer, int length);
extern void splitfilename(const char *name, char *fname, char *ext);
extern int fexist(const char *name);
extern size_t fsize(FILE *stream);
extern size_t freadline(char *buffer, size_t size, FILE *stream);
extern size_t freadstring(char *buffer, size_t size, FILE *stream);
extern size_t fwritestring(const char *buffer, size_t size, FILE *stream);

extern void LogOutput(const char *szTag, const char *szFormat, ...);

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
extern int stricmp(const char *src, const char *dst);
extern int strnicmp(const char *src, const char *dst, int count);

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

extern long _findfirst(const char *pattern, struct _finddata_t *data);
extern long _findnext(long id, struct _finddata_t *data);
extern long _findclose(long id);
#endif
