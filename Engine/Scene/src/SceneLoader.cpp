#include "SceneHeader.h"
#include "EngineHeader.h"


CSceneLoader* CSceneLoader::pInstance = nullptr;
CSceneLoader* CSceneLoader::GetInstance(void)
{
	return pInstance;
}

CSceneLoader::CSceneLoader(void)
{
	pInstance = this;
}

CSceneLoader::~CSceneLoader(void)
{
	pInstance = nullptr;
}
