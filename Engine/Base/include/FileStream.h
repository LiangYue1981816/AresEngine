#pragma once
#include "PreHeader.h"


class CALL_API CFileStream
{
public:
	CFileStream(void);
	virtual ~CFileStream(void);


public:
	bool IsValid(void) const;

public:
	bool LoadFromFile(const char* szFileName);
	bool LoadFromPack(ZZIP_DIR* pPack, const char* szFileName);
	void Close(void);

public:
	size_t GetFullSize(void) const;
	size_t GetFreeSize(void) const;
	size_t GetCurrentPosition(void) const;

public:
	size_t Read(void* pBuffer, size_t size, size_t count);
	bool Seek(int offset, int origin);
	bool IsEOF(void) const;


private:
	FILE* m_pFile;
	ZZIP_DIR* m_pPack;
	ZZIP_FILE* m_pPackFile;

private:
	static const int BUFFER_SIZE = 1 * 1024;

	uint8_t* m_pBuffer;

	int m_fileSize;
	int m_filePosition;

	int m_bufferSize;
	int m_bufferOffset;
	int m_bufferPosition;
	int m_bufferCacheSize;
	bool m_bNeedUpdateCache;
};
