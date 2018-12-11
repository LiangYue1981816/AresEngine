#pragma once
#include "GfxHeader.h"
#include "SceneHeader.h"


#define ResourceLoader() CResourceLoader::GetInstance()


class CALL_API CResourceLoader
{
public:
	static CResourceLoader* GetInstance(void);


public:
	CResourceLoader(void);
	virtual ~CResourceLoader(void);


public:
	virtual bool LoadShader(const char *szFileName, CGfxShader *pShader, shader_kind kind);
	virtual bool LoadMesh(const char *szFileName, CGfxMesh *pMesh, uint32_t instanceFormat);
	virtual bool LoadMaterial(const char *szFileName, CGfxMaterial *pMaterial);
	virtual bool LoadTexture2D(const char *szFileName, CGfxTexture2D *pTexture2D);
	virtual bool LoadTexture2DArray(const char *szFileName, CGfxTexture2DArray *pTexture2DArray);
	virtual bool LoadTexture2DArrayLayer(const char *szFileName, int layer, CGfxTexture2DArray *pTexture2DArray);
	virtual bool LoadTextureCubeMap(const char *szFileName, CGfxTextureCubeMap *pTextureCubeMap);
	virtual bool LoadTextureCubeMapFace(const char *szFileName, GfxTextureCubeMapFace face, CGfxTextureCubeMap *pTextureCubeMap);

public:
	virtual CSceneNode* LoadSceneMesh(const char *szFileName, CSceneNode *pParentSceneNode, uint32_t instanceFormat);


protected:
	static CResourceLoader *pInstance;
};
