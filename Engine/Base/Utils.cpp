#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "windows.h"


unsigned int tick(void)
{
#ifdef _WINDOWS

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

unsigned int HashValue(const unsigned char *pBuffer, int length)
{
	const unsigned char *c = pBuffer;
	unsigned int dwHashValue = 0x00000000;

	while (length--) {
//		dwHashValue = (dwHashValue << 5) - dwHashValue + (*c == '/' ? '\\' : *c); c++;
		dwHashValue = (dwHashValue << 4) - dwHashValue + (*c == '/' ? '\\' : *c); c++;
	}

	return dwHashValue ? dwHashValue : 0xffffffff;
}

void splitfilename(const char *name, char *fname, char *ext)
{
	const char *p = NULL;
	const char *c = NULL;
	const char *base = name;

	for (p = base; *p; p++) {
		if (*p == '/' || *p == '\\') {
			do {
				p++;
			} while (*p == '/' || *p == '\\');

			base = p;
		}
	}

	size_t len = strlen(base);
	for (p = base + len; p != base && *p != '.'; p--);
	if (p == base && *p != '.') p = base + len;

	if (fname) {
		for (c = base; c < p; c++) {
			fname[c - base] = *c;
		}

		fname[c - base] = 0;
	}

	if (ext) {
		for (c = p; c < base + len; c++) {
			ext[c - p] = *c;
		}

		ext[c - p] = 0;
	}
}

size_t fsize(FILE *stream)
{
	long pos;
	size_t size;

	pos = ftell(stream);
	{
		fseek(stream, 0, SEEK_END);
		size = ftell(stream);
	}
	fseek(stream, pos, SEEK_SET);

	return size;
}

size_t freadline(char *buffer, size_t size, FILE *stream)
{
	char c;
	size_t count = 0;

	while (feof(stream) == FALSE && count < size - 1) {
		c = fgetc(stream);

		if (c == '\r') {
			c = fgetc(stream);
		}

		if (c == '\n') {
			break;
		}

		if (c == -1) {
			break;
		}

		*buffer = c;

		buffer++;
		count++;
	}

	*buffer = 0;

	return count;
}

size_t freadstring(char *buffer, size_t size, FILE *stream)
{
	size_t len = 0;
	size_t reads = 0;

	reads += fread(&len, sizeof(len), 1, stream);
	reads += fread(buffer, sizeof(*buffer), min(len, size), stream);
	buffer[min(len, size)] = 0;

	return reads;
}

size_t fwritestring(const char *buffer, size_t size, FILE *stream)
{
	size_t len = 0;
	size_t writes = 0;

	len = min(strlen(buffer), size);
	writes += fwrite(&len, sizeof(len), 1, stream);
	writes += fwrite(buffer, sizeof(*buffer), len, stream);

	return writes;
}

void LogOutput(const char *szTag, const char *szFormat, ...)
{
	static char szText[128 * 1024];

	va_list vaList;
	va_start(vaList, szFormat);
	vsprintf(szText, szFormat, vaList);
	va_end(vaList);

#ifdef _WINDOWS

	if (szTag) {
		OutputDebugString(szTag);
		OutputDebugString(": ");
	}

	OutputDebugString(szText);

#endif

	if (szTag) {
		printf("%s: ", szTag);
	}

	printf("%s", szText);
}
