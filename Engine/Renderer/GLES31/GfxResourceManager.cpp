#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxResourceManager.h"


CGfxResourceManager::CGfxResourceManager(void)
{

}

CGfxResourceManager::~CGfxResourceManager(void)
{

}

void CGfxResourceManager::SetResourcePath(const char *szPathName, const char *szExtName)
{
	char szFilter[_MAX_STRING];
	sprintf(szFilter, "%s/*", szPathName);

	struct _finddata_t fileData;
	size_t hFile = (size_t)_findfirst(szFilter, &fileData);

	if (hFile != -1) {
		do {
			if (fileData.attrib&_A_SUBDIR) {
				if (!stricmp(fileData.name, ".")) continue;
				if (!stricmp(fileData.name, "..")) continue;

				char szSubPathName[_MAX_STRING];
				sprintf(szSubPathName, "%s/%s", szPathName, fileData.name);
				SetResourcePath(szSubPathName, szExtName);

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

			m_strResourceFullNames[HashValue(szFileName)] = szFullName;
		} while (_findnext(hFile, &fileData) == 0);

		_findclose(hFile);
	}
}

const char* CGfxResourceManager::GetResourceFullName(const char *szFileName) const
{
	const auto &itResourceFullName = m_strResourceFullNames.find(HashValue(szFileName));
	return itResourceFullName != m_strResourceFullNames.end() ? itResourceFullName->second.c_str() : NULL;
}
