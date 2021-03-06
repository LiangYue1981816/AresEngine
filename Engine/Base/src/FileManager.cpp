#include "BaseHeader.h"


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
	pInstance = nullptr;
}

void CFileManager::Clearup(void)
{
	for (const auto& itPack : m_packs) {
		zzip_closedir(itPack.second);
	}

	m_files.clear();
	m_packs.clear();
}

void CFileManager::SetPath(const char* szPathName, const char* szExtName)
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
			if (fileData.attrib & _A_SUBDIR) {
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

			m_files[HashValue(szFileName)].Set(nullptr, szFullName);
		} while (_findnext(hFile, &fileData) == 0);

		_findclose(hFile);
	}
}

void CFileManager::SetPack(const char* szPackName, const char* szExtName)
{
	ZZIP_DIR* pPack = nullptr;
	{
		uint32_t name = HashValue(szPackName);

		if (m_packs[name] == nullptr) {
			m_packs[name] = zzip_opendir(szPackName);
		}

		pPack = m_packs[name];
	}

	zzip_seekdir(pPack, 0);

	while (ZZIP_DIRENT* pEntry = zzip_readdir(pPack)) {
		char szFName[_MAX_STRING];
		char szEName[_MAX_STRING];
		splitfilename(pEntry->d_name, szFName, szEName);
		if (stricmp(szExtName, szEName)) continue;

		char szFileName[_MAX_STRING];
		char szFullName[_MAX_STRING];
		sprintf(szFileName, "%s%s", szFName, szEName);
		sprintf(szFullName, "%s", pEntry->d_name);

		m_files[HashValue(szFileName)].Set(pPack, szFullName);
	}
}

void CFileManager::SetFile(const char* szFileName, const char* szFullName)
{
	m_files[HashValue(szFileName)].Set(nullptr, szFullName);
}

const char* CFileManager::GetFullName(const char* szFileName)
{
	return GetFullName(HashValue(szFileName));
}

const char* CFileManager::GetFullName(uint32_t name)
{
	const auto& itFile = m_files.find(name);

	if (itFile != m_files.end()) {
		return itFile->second.strFullName.c_str();
	}
	else {
		return "";
	}
}

bool CFileManager::LoadStream(const char* szFileName, CStream* pStream)
{
	return LoadStream(HashValue(szFileName), pStream);
}

bool CFileManager::LoadStream(uint32_t name, CStream* pStream)
{
	const auto& itFile = m_files.find(name);

	if (itFile != m_files.end()) {
		if (pStream->LoadFromFile(itFile->second.strFullName.c_str())) {
			return true;
		}

		if (pStream->LoadFromPack(itFile->second.pPack, itFile->second.strFullName.c_str())) {
			return true;
		}
	}

	return false;
}
