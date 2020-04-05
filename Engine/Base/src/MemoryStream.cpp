#include "BaseHeader.h"


CMemoryStream::CMemoryStream(void)
	: m_bAlloced(false)
	, m_pBuffer(nullptr)

	, m_bufferSize(0)
	, m_bufferPosition(0)
{

}

CMemoryStream::~CMemoryStream(void)
{
	Close();
}

bool CMemoryStream::IsValid(void) const
{
	return m_pBuffer != nullptr;
}

bool CMemoryStream::SetStream(uint8_t* pBuffer, size_t size)
{
	if (pBuffer == nullptr) {
		return false;
	}

	if (size == 0) {
		return false;
	}

	if (IsValid()) {
		return false;
	}

	m_bAlloced = false;
	m_pBuffer = pBuffer;

	m_bufferSize = size;
	m_bufferPosition = 0;

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
			size_t size = fsize(pFile);

			if (size == 0) {
				break;
			}

			m_bAlloced = true;
			m_pBuffer = new uint8_t[size];

			m_bufferSize = size;
			m_bufferPosition = 0;

			fread(m_pBuffer, 1, size, pFile);
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

			size_t size = zstat.st_size;

			if (size == 0) {
				break;
			}

			m_bAlloced = true;
			m_pBuffer = new uint8_t[size];

			m_bufferSize = size;
			m_bufferPosition = 0;

			zzip_file_read(pFile, m_pBuffer, size);
			zzip_file_close(pFile);

			return true;
		} while (false);

		zzip_file_close(pFile);
	}

	return false;
}

void CMemoryStream::Close(void)
{
	if (m_bAlloced) {
		delete[] m_pBuffer;
	}

	m_bAlloced = false;
	m_pBuffer = nullptr;

	m_bufferSize = 0;
	m_bufferPosition = 0;
}

size_t CMemoryStream::GetFullSize(void) const
{
	if (IsValid()) {
		return m_bufferSize;
	}
	else {
		return 0;
	}
}

size_t CMemoryStream::GetFreeSize(void) const
{
	if (IsValid()) {
		return m_bufferSize - m_bufferPosition;
	}
	else {
		return 0;
	}
}

size_t CMemoryStream::GetCurrentPosition(void) const
{
	if (IsValid()) {
		return m_bufferPosition;
	}
	else {
		return 0;
	}
}

void* CMemoryStream::GetAddress(void) const
{
	if (IsValid()) {
		return m_pBuffer;
	}
	else {
		return nullptr;
	}
}

void* CMemoryStream::GetCurrentAddress(void) const
{
	if (IsValid()) {
		return m_pBuffer + m_bufferPosition;
	}
	else {
		return nullptr;
	}
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

	memcpy(pBuffer, m_pBuffer + m_bufferPosition, readSize);
	m_bufferPosition += readSize;

	return readSize / size;
}

bool CMemoryStream::Seek(int offset, int origin)
{
	if (IsValid() == false) {
		return false;
	}

	switch (origin) {
	case SEEK_CUR:
		if (m_bufferPosition + offset < 0) {
			return false;
		}

		if (m_bufferPosition + offset > m_bufferSize) {
			return false;
		}

		m_bufferPosition = m_bufferPosition + offset;

		break;
	case SEEK_END:
		if (m_bufferSize + offset < 0) {
			return false;
		}

		if (m_bufferSize + offset > m_bufferSize) {
			return false;
		}

		m_bufferPosition = m_bufferSize + offset;

		break;
	case SEEK_SET:
		if (offset < 0) {
			return false;
		}

		if (offset > m_bufferSize) {
			return false;
		}

		m_bufferPosition = offset;

		break;
	}

	return true;
}

bool CMemoryStream::IsEOF(void) const
{
	return GetFreeSize() == 0;
}
