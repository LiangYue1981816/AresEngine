#pragma once
#include "SceneHeader.h"


#define SceneLoader() CSceneLoader::GetInstance()


class CALL_API CSceneLoader
{
public:
	static CSceneLoader* GetInstance(void);


public:
	CSceneLoader(void);
	virtual ~CSceneLoader(void);


public:
	CSceneNode* LoadMesh(const char *szFileName, CSceneNode *pParentSceneNode);


protected:
	static CSceneLoader *pInstance;
};
