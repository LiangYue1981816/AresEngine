#include "Utils.h"
#include "Stream.h"
#include "FileManager.h"


CFileManager* CFileManager::pInstance = nullptr;
CFileManager* CFileManager::GetInstance(void)
{
	return pInstance;
}

CFileManager::CFileManager(void)
{
	pInstance = this;
}

CFileManager::~CFileManager(void)
{
	Clearup();
}

void CFileManager::Clearup(void)
{
	for (const auto &itPack : m_packs) {
		zzip_closedir(itPack.second);
	}

	m_files.clear();
	m_packs.clear();
}

void CFileManager::SetPath(const char *szPathName, const char *szExtName)
{
#ifdef PLATFORM_WINDOWS
	char szFilter[_MAX_STRING];
	sprintf(szFilter, "%s/*", szPathName);
#else
	char szFilter[_MAX_STRING];
	sprintf(szFilter, "%s/", szPathName);
#endif

	struct _finddata_t fileData;
	size_t hFile = (size_t)_findfirst(szFilter, &fileData);

	if (hFile != -1) {
		do {
			if (fileData.attrib&_A_SUBDIR) {
				if (!stricmp(fileData.name, ".")) continue;
				if (!stricmp(fileData.name, "..")) continue;

				char szSubPathName[_MAX_STRING];
				sprintf(szSubPathName, "%s/%s", szPathName, fileData.name);
				SetPath(szSubPathName, szExtName);

				continue;
			}

			char szFName[_MAX_STRING];
			char szEName[_MAX_STRING];
			splitfilename(fileData.name, szFName, szEName);
			if (stricmp(szExtName, szEName)) continue;

			char szFileName[_MAX_STRING];
			char szFullName[_MAX_STRING];
			sprintf(szFileName, "%s%s", szFName, szEName);
			sprintf(szFullName, "%s/%s", szPathName, szFileName);

			uint32_t name = HashValue(szFileName);
			m_files[name].pPack = nullptr;
			strcpy(m_files[name].szFileName, szFileName);
			strcpy(m_files[name].szFullName, szFullName);
		} while (_findnext(hFile, &fileData) == 0);

		_findclose(hFile);
	}
}

void CFileManager::SetPack(const char *szPackName, const char *szExtName)
{
	ZZIP_DIR *pPack = nullptr;
	{
		uint32_t name = HashValue(szPackName);

		if (m_packs[name] == nullptr) {
			m_packs[name] = zzip_opendir(szPackName);
		}

		pPack = m_packs[name];
	}

	zzip_seekdir(pPack, 0);

	while (ZZIP_DIRENT *pEntry = zzip_readdir(pPack)) {
		char szFName[_MAX_STRING];
		char szEName[_MAX_STRING];
		splitfilename(pEntry->d_name, szFName, szEName);
		if (stricmp(szExtName, szEName)) continue;

		char szFileName[_MAX_STRING];
		char szFullName[_MAX_STRING];
		sprintf(szFileName, "%s%s", szFName, szEName);
		sprintf(szFullName, "%s", pEntry->d_name);

		uint32_t name = HashValue(szFileName);
		m_files[name].pPack = pPack;
		strcpy(m_files[name].szFileName, szFileName);
		strcpy(m_files[name].szFullName, szFullName);
	}
}

void CFileManager::SetFile(const char *szFileName, const char *szFullName)
{
	uint32_t name = HashValue(szFileName);
	m_files[name].pPack = nullptr;
	strcpy(m_files[name].szFileName, szFileName);
	strcpy(m_files[name].szFullName, szFullName);
}

const char* CFileManager::GetFullName(const char *szFileName)
{
	const auto &itFile = m_files.find(HashValue(szFileName));

	if (itFile != m_files.end()) {
		return itFile->second.szFullName;
	}
	else {
		return "";
	}
}

bool CFileManager::LoadStream(const char *szFileName, CStream *pStream)
{
	const auto &itFile = m_files.find(HashValue(szFileName));

	if (itFile != m_files.end()) {
		if (pStream->LoadFromFile(itFile->second.szFullName)) {
			return true;
		}

		if (pStream->LoadFromPack(itFile->second.pPack, itFile->second.szFullName)) {
			return true;
		}
	}

	return false;
}
