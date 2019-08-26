#pragma once
#include "PreHeader.h"
#include "zzip/zzip.h"


#define CreateFileManager() CFileManager::Create()
#define DestroyFileManager() CFileManager::Destroy()
#define FileManager() CFileManager::GetInstance()


class CALL_API CStream;
class CALL_API CFileManager
{
	friend class CEngine;


private:
	typedef struct File {
		ZZIP_DIR* pPack = nullptr;
		eastl::string strFullName;
	} File;


public:
	static CFileManager* GetInstance(void);
	static void Create(void);
	static void Destroy(void);


private:
	CFileManager(void);
	virtual ~CFileManager(void);


public:
	void Clearup(void);

	void SetPath(const char* szPathName, const char* szExtName);
	void SetPack(const char* szPackName, const char* szExtName);
	void SetFile(const char* szFileName, const char* szFullName);

	const char* GetFullName(const char* szFileName);
	const char* GetFullName(uint32_t name);

	bool LoadStream(const char* szFileName, CStream* pStream);
	bool LoadStream(uint32_t name, CStream* pStream);


private:
	eastl::unordered_map<uint32_t, File> m_files;
	eastl::unordered_map<uint32_t, ZZIP_DIR*> m_packs;

private:
	static CFileManager* pInstance;
};
