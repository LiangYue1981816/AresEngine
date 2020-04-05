#include "BaseHeader.h"


CMemoryStream::CMemoryStream(void)
	: m_bAlloced(false)
	, m_pAddress(nullptr)

	, m_size(0)
	, m_position(0)
{

}

CMemoryStream::~CMemoryStream(void)
{
	Free();
}

bool CMemoryStream::IsValid(void) const
{
	return m_pAddress != nullptr && m_size > 0;
}

bool CMemoryStream::Alloc(size_t size)
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

void CMemoryStream::Free(void)
{
	if (m_bAlloced) {
		delete[] m_pAddress;
	}

	m_bAlloced = false;
	m_pAddress = nullptr;

	m_size = 0;
	m_position = 0;
}

bool CMemoryStream::SetStream(uint8_t* pAddress, size_t size)
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

bool CMemoryStream::LoadFromFile(const char* szFileName)
{
	if (szFileName == nullptr) {
		return false;
	}

	if (IsValid()) {
		return false;
	}

	if (FILE* pFile = fopen(szFileName, "rb")) {
		do {
			if (Alloc(fsize(pFile)) == false) {
				break;
			}

			if (m_size != fread(m_pAddress, 1, m_size, pFile)) {
				break;
			}

			fclose(pFile);

			return true;
		} while (false);

		fclose(pFile);
	}

	return false;
}

bool CMemoryStream::LoadFromPack(ZZIP_DIR* pPack, const char* szFileName)
{
	if (pPack == nullptr) {
		return false;
	}

	if (szFileName == nullptr) {
		return false;
	}

	if (IsValid()) {
		return false;
	}

	if (ZZIP_FILE* pFile = zzip_file_open(pPack, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS)) {
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

			zzip_file_close(pFile);

			return true;
		} while (false);

		zzip_file_close(pFile);
	}

	return false;
}

size_t CMemoryStream::GetFullSize(void) const
{
	return m_size;
}

size_t CMemoryStream::GetFreeSize(void) const
{
	return m_size - m_position;
}

size_t CMemoryStream::GetCurrentPosition(void) const
{
	return m_position;
}

void* CMemoryStream::GetAddress(void) const
{
	return m_pAddress;
}

void* CMemoryStream::GetCurrentAddress(void) const
{
	return m_pAddress + m_position;
}

size_t CMemoryStream::Read(void* pBuffer, size_t size, size_t count)
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

bool CMemoryStream::Seek(int offset, int origin)
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

bool CMemoryStream::IsEOF(void) const
{
	return GetFreeSize() == 0;
}