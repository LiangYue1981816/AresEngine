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
	bool Alloc(size_t size);
	void Free(void);

	bool CopyFrom(const CFileStream* pStream);
	bool SetStream(uint8_t* pAddress, size_t size);

public:
	bool LoadFromFile(const char* szFileName);
	bool LoadFromPack(const char* szPackName, const char* szFileName);
	bool LoadFromPack(ZZIP_DIR* pPack, const char* szFileName);

	bool Reload(void);

public:
	bool SetName(const char* szName);
	const char* GetName(void) const;
	uint32_t GetHashName(void) const;

	bool SetFileName(const char* szFileName);
	const char* GetFileName(void) const;

	bool SetPackName(const char* szPackName);
	const char* GetPackName(void) const;

	bool SetPack(ZZIP_DIR* pPack);
	ZZIP_DIR* GetPack(void) const;

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
	uint32_t m_dwName;
	char m_szName[_MAX_STRING];

	ZZIP_DIR* m_pPack;
	char m_szPackName[_MAX_STRING];
	char m_szFileName[_MAX_STRING];

private:
	bool m_bAlloced;
	uint8_t* m_pAddress;

	int m_size;
	int m_position;
};
