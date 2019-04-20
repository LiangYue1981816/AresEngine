#include "Utils.h"
#include "Stream.h"


CStream::CStream(void)
	: m_dwName(INVALID_HASHNAME)
	, m_szName{ 0 }

	, m_pPack(nullptr)
	, m_szPackName{ 0 }
	, m_szFileName{ 0 }

	, m_bAlloced(false)
	, m_pAddress(nullptr)

	, m_size(0)
	, m_position(0)
{

}

CStream::~CStream(void)
{
	Free();
}

bool CStream::Alloc(size_t size)
{
	if (size == 0) {
		return false;
	}

	if (IsValid()) {
		return false;
	}

	m_bAlloced = true;
	m_pAddress = new uint8_t[size];

	m_size = size;
	m_position = 0;

	return true;
}

void CStream::Free(void)
{
	if (m_bAlloced) {
		delete[] m_pAddress;
	}

	m_bAlloced = false;
	m_pAddress = nullptr;

	m_size = 0;
	m_position = 0;
}

bool CStream::IsValid(void) const
{
	return m_pAddress != nullptr && m_size > 0;
}

bool CStream::SetStream(uint8_t* pAddress, size_t size)
{
	if (pAddress == nullptr) {
		return false;
	}

	if (size == 0) {
		return false;
	}

	if (IsValid()) {
		return false;
	}

	m_bAlloced = false;
	m_pAddress = pAddress;

	m_size = size;
	m_position = 0;

	return true;
}

bool CStream::CopyFrom(const CStream* pStream)
{
	Free();

	if (pStream == nullptr) {
		return false;
	}

	if (pStream->IsValid() == false) {
		return false;
	}

	if (Alloc(pStream->GetFullSize()) == false) {
		return false;
	}

	SetName(pStream->GetName());
	SetPack(pStream->GetPack());
	SetPackName(pStream->GetPackName());
	SetFileName(pStream->GetFileName());
	memcpy(m_pAddress, pStream->GetAddress(), pStream->GetFullSize());

	return true;
}

bool CStream::LoadFromFile(const char* szFileName)
{
	Free();
	{
		if (szFileName == nullptr) {
			return false;
		}

		if (FILE * pFile = fopen(szFileName, "rb")) {
			do {
				if (Alloc(fsize(pFile)) == false) {
					break;
				}

				if (m_size != fread(m_pAddress, 1, m_size, pFile)) {
					break;
				}

				SetFileName(szFileName);
				fclose(pFile);

				return true;
			} while (false);

			fclose(pFile);
		}
	}
	Free();
	return false;
}

bool CStream::LoadFromPack(const char* szPackName, const char* szFileName)
{
	Free();
	{
		if (szPackName == nullptr) {
			return false;
		}

		if (szFileName == nullptr) {
			return false;
		}

		if (ZZIP_DIR * pPack = zzip_opendir(szPackName)) {
			do {
				if (LoadFromPack(pPack, szFileName) == false) {
					break;
				}

				SetPackName(szPackName);
				SetFileName(szFileName);
				zzip_closedir(pPack);

				return true;
			} while (false);

			zzip_closedir(pPack);
		}
	}
	Free();
	return false;
}

bool CStream::LoadFromPack(ZZIP_DIR* pPack, const char* szFileName)
{
	Free();
	{
		if (pPack == nullptr) {
			return false;
		}

		if (szFileName == nullptr) {
			return false;
		}

		if (ZZIP_FILE * pFile = zzip_file_open(pPack, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS)) {
			do {
				ZZIP_STAT zstat;

				if (zzip_file_stat(pFile, &zstat) != ZZIP_NO_ERROR) {
					break;
				}

				if (Alloc(zstat.st_size) == false) {
					break;
				}

				if (m_size != zzip_file_read(pFile, m_pAddress, m_size)) {
					break;
				}

				SetPack(pPack);
				SetFileName(szFileName);
				zzip_file_close(pFile);

				return true;
			} while (false);

			zzip_file_close(pFile);
		}
	}
	Free();
	return false;
}

bool CStream::Reload(void)
{
	ZZIP_DIR* pPack = m_pPack;

	char szPackName[_MAX_STRING];
	char szFileName[_MAX_STRING];

	strcpy(szPackName, m_szPackName);
	strcpy(szFileName, m_szFileName);

	if (strlen(szFileName)) {
		if (pPack) {
			return LoadFromPack(pPack, szFileName);
		}

		if (strlen(szPackName)) {
			return LoadFromPack(szPackName, szFileName);
		}

		return LoadFromFile(szFileName);
	}

	return false;
}

bool CStream::SetName(const char* szName)
{
	if (szName == nullptr) {
		return false;
	}

	strcpy(m_szName, szName);
	m_dwName = HashValue(m_szName);

	return true;
}

const char* CStream::GetName(void) const
{
	return m_szName;
}

uint32_t CStream::GetHashName(void) const
{
	return m_dwName;
}

bool CStream::SetFileName(const char* szFileName)
{
	if (szFileName == nullptr) {
		return false;
	}

	char szName[_MAX_STRING];
	char szFName[_MAX_STRING];
	char szEName[_MAX_STRING];

	strcpy(m_szFileName, szFileName);
	splitfilename(m_szFileName, szFName, szEName);
	sprintf(szName, "%s%s", szFName, szEName);

	SetName(szName);

	return true;
}

const char* CStream::GetFileName(void) const
{
	return m_szFileName;
}

bool CStream::SetPackName(const char* szPackName)
{
	if (szPackName == nullptr) {
		return false;
	}

	strcpy(m_szPackName, szPackName);

	return true;
}

const char* CStream::GetPackName(void) const
{
	return m_szPackName;
}

bool CStream::SetPack(ZZIP_DIR* pPack)
{
	if (pPack == nullptr) {
		return false;
	}

	m_pPack = pPack;

	return true;
}

ZZIP_DIR* CStream::GetPack(void) const
{
	return m_pPack;
}

size_t CStream::GetFullSize(void) const
{
	return m_size;
}

size_t CStream::GetFreeSize(void) const
{
	return m_size - m_position;
}

size_t CStream::GetCurrentPosition(void) const
{
	return m_position;
}

void* CStream::GetAddress(void) const
{
	return m_pAddress;
}

void* CStream::GetCurrentAddress(void) const
{
	return m_pAddress + m_position;
}

size_t CStream::Read(void* pBuffer, size_t size, size_t count)
{
	if (pBuffer == nullptr) {
		return 0;
	}

	if (size == 0 || count == 0) {
		return 0;
	}

	if (IsValid() == false) {
		return 0;
	}

	size_t readSize;

	readSize = size * count;
	readSize = std::min(readSize, GetFreeSize());

	memcpy(pBuffer, m_pAddress + m_position, readSize);
	m_position += readSize;

	return readSize / size;
}

bool CStream::Seek(int offset, int origin)
{
	if (IsValid() == false) {
		return false;
	}

	switch (origin) {
	case SEEK_CUR:
		if (m_position + offset < 0) {
			return false;
		}

		if (m_position + offset > m_size) {
			return false;
		}

		m_position = m_position + offset;

		break;
	case SEEK_END:
		if (m_size + offset < 0) {
			return false;
		}

		if (m_size + offset > m_size) {
			return false;
		}

		m_position = m_size + offset;

		break;
	case SEEK_SET:
		if (offset < 0) {
			return false;
		}

		if (offset > m_size) {
			return false;
		}

		m_position = offset;

		break;
	}

	return true;
}

bool CStream::IsEOF(void) const
{
	return GetFreeSize() == 0;
}
