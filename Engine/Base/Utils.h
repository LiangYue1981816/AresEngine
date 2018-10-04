#pragma once
#include <stdio.h>
#include <stdlib.h>


extern unsigned int tick(void);
extern unsigned int HashValue(const char *szString);
extern unsigned int HashValue(const unsigned char *pBuffer, int length);
extern void splitfilename(const char *name, char *fname, char *ext);
extern size_t fsize(FILE *stream);
extern size_t freadline(char *buffer, size_t size, FILE *stream);
extern size_t freadstring(char *buffer, size_t size, FILE *stream);
extern size_t fwritestring(const char *buffer, size_t size, FILE *stream);
extern void LogOutput(const char *szTag, const char *szFormat, ...);
