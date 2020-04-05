#include "BaseHeader.h"


CFileStream::CFileStream(void)
	: m_pFile(nullptr)
	, m_pPack(nullptr)
	, m_pPackFile(nullptr)

	, m_pBuffer(nullptr)

	, m_fileSize(0)
	, m_filePosition(0)

	, m_bufferSize(0)
	, m_bufferOffset(0)
	, m_bufferPosition(0)
	, m_bufferCacheSize(0)
{

}

CFileStream::~CFileStream(void)
{
	Close();
}

bool CFileStream::IsValid(void) const
{
	return (m_pBuffer != nullptr) && ((m_pFile != nullptr) || (m_pPack != nullptr && m_pPackFile != nullptr));
}

bool CFileStream::Alloc(size_t size)
{
	if (size == 0) {
		return false;
	}

	m_pBuffer = new uint8_t[size];

	m_bufferSize = size;
	m_bufferOffset = 0;
	m_bufferPosition = 0;
	m_bufferCacheSize = 0;

	return true;
}

void CFileStream::Free(void)
{
	delete[] m_pBuffer;
	m_pBuffer = nullptr;

	m_bufferSize = 0;
	m_bufferOffset = 0;
	m_bufferPosition = 0;
	m_bufferCacheSize = 0;
}

bool CFileStream::LoadFromFile(const char* szFileName)
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

			m_pFile = pFile;

			m_fileSize = size;
			m_filePosition = 0;

			Alloc(std::min((size_t)CACHE_SIZE, GetFullSize()));

			return true;
		} while (false);

		fclose(pFile);
	}

	return false;
}

bool CFileStream::LoadFromPack(ZZIP_DIR* pPack, const char* szFileName)
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

			m_pPack = pPack;
			m_pPackFile = pFile;

			m_fileSize = zstat.st_size;
			m_filePosition = 0;

			Alloc(std::min((size_t)CACHE_SIZE, GetFullSize()));

			return true;
		} while (false);

		zzip_file_close(pFile);
	}

	return false;
}

void CFileStream::Close(void)
{
	if (m_pFile) {
		fclose(m_pFile);
	}

	if (m_pPack && m_pPackFile) {
		zzip_file_close(m_pPackFile);
	}

	m_pFile = nullptr;
	m_pPack = nullptr;
	m_pPackFile = nullptr;

	m_fileSize = 0;
	m_filePosition = 0;

	Free();
}

size_t CFileStream::GetFullSize(void) const
{
	if (IsValid()) {
		return m_fileSize;
	}
	else {
		return 0;
	}
}

size_t CFileStream::GetFreeSize(void) const
{
	if (IsValid()) {
		return m_fileSize - m_bufferOffset - m_bufferPosition;
	}
	else {
		return 0;
	}
}

size_t CFileStream::GetCurrentPosition(void) const
{
	if (IsValid()) {
		return m_bufferOffset + m_bufferPosition;
	}
	else {
		return 0;
	}
}

size_t CFileStream::Read(void* pBuffer, size_t size, size_t count)
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

	size_t readSize = 0;
	size_t copySize = 0;

	readSize = size * count;
	readSize = std::min(readSize, GetFreeSize());

	do {

	} while (copySize != readSize);

	return readSize / size;
}

bool CFileStream::Seek(int offset, int origin)
{
	if (IsValid() == false) {
		return false;
	}

	switch (origin) {
	case SEEK_CUR:
		if (m_filePosition + offset < 0) {
			return false;
		}

		if (m_filePosition + offset > m_fileSize) {
			return false;
		}

		m_filePosition = m_filePosition + offset;

		break;
	case SEEK_END:
		if (m_fileSize + offset < 0) {
			return false;
		}

		if (m_fileSize + offset > m_fileSize) {
			return false;
		}

		m_filePosition = m_fileSize + offset;

		break;
	case SEEK_SET:
		if (offset < 0) {
			return false;
		}

		if (offset > m_fileSize) {
			return false;
		}

		m_filePosition = offset;

		break;
	}

	if (m_pFile) {
		fseek(m_pFile, m_filePosition, SEEK_SET);
	}

	if (m_pPack && m_pPackFile) {
		zzip_seek(m_pPackFile, m_filePosition, SEEK_SET);
	}

	return true;
}

bool CFileStream::IsEOF(void) const
{
	return GetFreeSize() == 0;
}
