#include "ResourceLoader.h"


CResourceLoader* CResourceLoader::pInstance = nullptr;
CResourceLoader* CResourceLoader::GetInstance(void)
{
	return pInstance;
}

CResourceLoader::CResourceLoader(void)
{
	pInstance = this;
}

CResourceLoader::~CResourceLoader(void)
{
	pInstance = nullptr;
}
