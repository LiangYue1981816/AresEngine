#pragma once
#include "PreHeader.h"


#define FileManager() CFileManager::GetInstance()


class CFileManager
{
	friend class CEngine;


private:
	typedef struct File {
		File(void)
		{
			pPack = nullptr;
			strFullName = "";
		}

		void Set(ZZIP_DIR* _pPack, const char* _szFullName)
		{
			pPack = _pPack;
			strFullName = _szFullName;
		}

		ZZIP_DIR* pPack;
		eastl::string strFullName;
	} File;


public:
	static CFileManager* GetInstance(void);


private:
	CFileManager(void);
	virtual ~CFileManager(void);


public:
	void Clearup(void);

	void SetPath(const char* szPathName, const char* szExtName);
	void SetPack(const char* szPackName, const char* szExtName);
	void SetFile(const char* szFileName, const char* szFullName);

public:
	const char* GetFullName(const char* szFileName);
	const char* GetFullName(uint32_t name);

public:
	bool LoadStream(const char* szFileName, CStream* pStream);
	bool LoadStream(uint32_t name, CStream* pStream);


private:
	eastl::unordered_map<uint32_t, File> m_files;
	eastl::unordered_map<uint32_t, ZZIP_DIR*> m_packs;

private:
	static CFileManager* pInstance;
};
