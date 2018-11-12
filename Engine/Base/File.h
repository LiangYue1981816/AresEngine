#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"


class CALL_API CFile
{
public:
	CFile(void);
	virtual ~CFile(void);


public:
	bool Open(const char *szFileName, const char *szMode);
	void Close(void);

	size_t Size(void);
	size_t Read(void *pBuffer, size_t size, size_t count);
	size_t Write(void *pBuffer, size_t size, size_t count);

	int Seek(long offset, int origin);
	int Eof(void);


private:
	FILE *m_pFile;
};
