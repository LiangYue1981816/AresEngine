#include "File.h"
#include "Utils.h"


CFile::CFile(void)
	: m_pFile(nullptr)
{

}

CFile::~CFile(void)
{
	Close();
}

bool CFile::Open(const char *szFileName, const char *szMode)
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

size_t CFile::Read(void *pBuffer, size_t size, size_t count)
{
	return m_pFile ? fread(pBuffer, size, count, m_pFile) : 0;
}

size_t CFile::Write(void *pBuffer, size_t size, size_t count)
{
	return m_pFile ? fwrite(pBuffer, size, count, m_pFile) : 0;
}

int CFile::Seek(long offset, int origin)
{
	return m_pFile ? fseek(m_pFile, offset, origin) : -1;
}

int CFile::Eof(void)
{
	return m_pFile ? feof(m_pFile) : -1;
}
