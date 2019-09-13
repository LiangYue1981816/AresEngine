#include "BaseHeader.h"


CFile::CFile(void)
	: m_pFile(nullptr)
{

}

CFile::~CFile(void)
{
	Close();
}

bool CFile::Open(const char* szFileName, const char* szMode)
{
	Close();

	m_pFile = fopen(szFileName, szMode);
	return m_pFile != nullptr;
}

void CFile::Close(void)
{
	if (m_pFile) {
		fclose(m_pFile);
	}
}

size_t CFile::Size(void)
{
	return fsize(m_pFile);
}

size_t CFile::Read(void* pBuffer, size_t size, size_t count)
{
	if (m_pFile) {
		return fread(pBuffer, size, count, m_pFile);
	}
	else {
		return 0;
	}
}

size_t CFile::Write(void* pBuffer, size_t size, size_t count)
{
	if (m_pFile) {
		return fwrite(pBuffer, size, count, m_pFile);
	}
	else {
		return 0;
	}
}

int CFile::Seek(long offset, int origin)
{
	if (m_pFile) {
		return fseek(m_pFile, offset, origin);
	}
	else {
		return -1;
	}
}

int CFile::Eof(void)
{
	if (m_pFile) {
		return feof(m_pFile);
	}
	else {
		return -1;
	}
}
