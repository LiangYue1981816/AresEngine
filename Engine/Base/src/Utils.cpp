#include "Utils.h"


CALL_API unsigned int NumCpuCores(void)
{
#ifdef PLATFORM_WINDOWS
	static SYSTEM_INFO sysInfo = {};
	if (sysInfo.dwNumberOfProcessors == 0) {
		GetSystemInfo(&sysInfo);
	}
	return sysInfo.dwNumberOfProcessors;
#else
	return sysconf(_SC_NPROCESSORS_CONF);
#endif
}

CALL_API unsigned int Tick(void)
{
#ifdef PLATFORM_WINDOWS
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

CALL_API unsigned int HashValue(const char *szString)
{
	const char *c = szString;
	unsigned int dwHashValue = 0x00000000;

	if (szString) {
		while (*c) {
			dwHashValue = (dwHashValue << 5) - dwHashValue + (*c == '/' ? '\\' : *c); c++;
		}
	}

	return dwHashValue ? dwHashValue : INVALID_HASHVALUE;
}

CALL_API unsigned int HashValue(const unsigned char *pBuffer, int length, int stride)
{
	const unsigned char *c = pBuffer;
	unsigned int dwHashValue = 0x00000000;

	if (pBuffer) {
		while (length > 0) {
			dwHashValue = (dwHashValue << 5) - dwHashValue + (*c); c += stride; length -= stride;
		}
	}

	return dwHashValue ? dwHashValue : INVALID_HASHVALUE;
}

CALL_API void LogOutput(const char *szTag, const char *szFormat, ...)
{
	static char szText[128 * 1024];

	va_list vaList;
	va_start(vaList, szFormat);
	vsprintf(szText, szFormat, vaList);
	va_end(vaList);

#ifdef PLATFORM_WINDOWS
	if (szTag) {
		OutputDebugString(szTag);
		OutputDebugString(": ");
		OutputDebugString(szText);
	}
	else {
		OutputDebugString(szText);
	}
#elif PLATFORM_ANDROID
	if (szTag) {
		__android_log_print(ANDROID_LOG_INFO, szTag, "%s", szText);
	}
	else {
		__android_log_print(ANDROID_LOG_INFO, "", "%s", szText);
	}
#else
	if (szTag) {
		printf("%s: %s", szTag, szText);
	}
	else {
		printf("%s", szText);
	}
#endif
}

CALL_API void splitfilename(const char *name, char *fname, char *ext)
{
	const char *p = nullptr;
	const char *c = nullptr;
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

CALL_API int fexist(const char *name)
{
	if (FILE *stream = fopen(name, "rb")) {
		fclose(stream);
		return 1;
	}

	return 0;
}

CALL_API size_t fsize(FILE *stream)
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

CALL_API size_t freadline(char *buffer, size_t size, FILE *stream)
{
	int c;
	size_t count = 0;

	while (feof(stream) == 0 && count < size - 1) {
		c = fgetc(stream);

		if (c == '\r') {
			c = fgetc(stream);
		}

		if (c == '\n') {
			break;
		}

		if (c == EOF) {
			break;
		}

		*buffer = (char)c;

		buffer++;
		count++;
	}

	*buffer = 0;

	return count;
}

CALL_API size_t freadstring(char *buffer, size_t size, FILE *stream)
{
	size_t len = 0;
	size_t reads = 0;

	reads += fread(&len, sizeof(len), 1, stream);
	reads += fread(buffer, sizeof(*buffer), std::min(len, size), stream);
	buffer[std::min(len, size)] = 0;

	return reads;
}

CALL_API size_t fwritestring(const char *buffer, size_t size, FILE *stream)
{
	size_t len = 0;
	size_t writes = 0;

	len = std::min(strlen(buffer), size);
	writes += fwrite(&len, sizeof(len), 1, stream);
	writes += fwrite(buffer, sizeof(*buffer), len, stream);

	return writes;
}

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)

CALL_API int stricmp(const char *src, const char *dst)
{
	int f, l;

	do {
		f = (unsigned char)(*(dst++));
		l = (unsigned char)(*(src++));

		if ((f >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if ((l >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (f && (f == l));

	return (f - l);
}

CALL_API int strnicmp(const char *src, const char *dst, int count)
{
	int f, l;

	do {
		f = (unsigned char)(*(dst++));
		l = (unsigned char)(*(src++));

		if ((f >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if ((l >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (f && (f == l) && --count);

	return (f - l);
}

static void _dirent2finddata(struct dirent *d, struct _finddata_t *data)
{
	data->attrib = 0;
	strcpy(data->name, d->d_name);

	switch (d->d_type) {
	case DT_FIFO:break;
	case DT_CHR: break;
	case DT_DIR: data->attrib = _A_SUBDIR; break;
	case DT_BLK: break;
	case DT_REG: data->attrib = _A_NORMAL; break;
	case DT_LNK: break;
	case DT_SOCK:break;
	case DT_WHT: break;
	}
}

CALL_API long _findfirst(const char *pattern, struct _finddata_t *data)
{
	DIR *id = opendir(pattern);
	if (id == nullptr) return -1;

	struct dirent *d = readdir((DIR *)id);
	if (d == nullptr) return -1;

	_dirent2finddata(d, data);
	return (long)id;
}

CALL_API long _findnext(long id, struct _finddata_t *data)
{
	struct dirent *d = readdir((DIR *)id);
	if (d == nullptr) return -1;

	_dirent2finddata(d, data);
	return 0;
}

CALL_API long _findclose(long id)
{
	closedir((DIR *)id);
	return 0;
}

#endif
