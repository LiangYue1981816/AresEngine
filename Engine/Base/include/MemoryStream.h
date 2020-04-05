#pragma once
#include "PreHeader.h"


class CALL_API CMemoryStream
{
public:
	CMemoryStream(void);
	virtual ~CMemoryStream(void);


public:
	bool IsValid(void) const;

public:
	bool SetStream(uint8_t* pBuffer, size_t size);
	bool LoadFromFile(const char* szFileName);
	bool LoadFromPack(ZZIP_DIR* pPack, const char* szFileName);
	void Close(void);

public:
	size_t GetFullSize(void) const;
	size_t GetFreeSize(void) const;
	size_t GetCurrentPosition(void) const;

	void* GetAddress(void) const;
	void* GetCurrentAddress(void) const;

public:
	size_t Read(void* pBuffer, size_t size, size_t count);
	bool Seek(int offset, int origin);
	bool IsEOF(void) const;


private:
	bool m_bAlloced;
	uint8_t* m_pBuffer;

	int m_bufferSize;
	int m_bufferPosition;
};
