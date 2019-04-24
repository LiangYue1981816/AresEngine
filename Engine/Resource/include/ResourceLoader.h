#pragma once
#include "GfxHeader.h"
#include "SceneHeader.h"


#define ResourceLoader() CResourceLoader::GetInstance()


class CALL_API CResourceLoader
{
	friend class CEngine;


public:
	static CResourceLoader* GetInstance(void);


private:
	CResourceLoader(void);
	virtual ~CResourceLoader(void);


public:
	virtual bool LoadShader(const char* szFileName, CGfxShader* pShader, shader_kind kind);
	virtual bool LoadMesh(const char* szFileName, CGfxMesh* pMesh, int vertexBinding = 0);
	virtual bool LoadMaterial(const char* szFileName, CGfxMaterial* pMaterial, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0);
	virtual bool LoadTexture2D(const char* szFileName, CGfxTexture2D* pTexture2D, int baseLevel);
	virtual bool LoadTexture2DArray(const char* szFileName, CGfxTexture2DArray* pTexture2DArray, int baseLevel);
	virtual bool LoadTexture2DArrayLayer(const char* szFileName, int layer, CGfxTexture2DArray* pTexture2DArray, int baseLevel);
	virtual bool LoadTextureCubeMap(const char* szFileName, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel);
	virtual bool LoadTextureCubeMapFace(const char* szFileName, GfxTextureCubeMapFace face, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel);

public:
	virtual CSceneNode* LoadSceneMesh(const char* szFileName, CSceneNode* pParentSceneNode, uint32_t instanceFormat, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0);


protected:
	static CResourceLoader* pInstance;
};
