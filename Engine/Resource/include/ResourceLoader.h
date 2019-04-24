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
	virtual bool LoadMesh(const char* szFileName, CGfxMesh* pMesh, int vertexBinding);
	virtual bool LoadMaterial(const char* szFileName, CGfxMaterial* pMaterial, int vertexBinding, int instanceBinding, int baseLevel, int numLevels);
	virtual bool LoadTexture2D(const char* szFileName, CGfxTexture2D* pTexture2D, int baseLevel, int numLevels);
	virtual bool LoadTexture2DArray(const char* szFileName, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels);
	virtual bool LoadTexture2DArrayLayer(const char* szFileName, int layer, CGfxTexture2DArray* pTexture2DArray, int baseLevel, int numLevels);
	virtual bool LoadTextureCubeMap(const char* szFileName, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel, int numLevels);
	virtual bool LoadTextureCubeMapFace(const char* szFileName, GfxTextureCubeMapFace face, CGfxTextureCubeMap* pTextureCubeMap, int baseLevel, int numLevels);

public:
	virtual CSceneNode* LoadSceneMesh(const char* szFileName, CSceneNode* pParentSceneNode, uint32_t instanceFormat, int vertexBinding, int instanceBinding, int baseLevel, int numLevels);


protected:
	static CResourceLoader* pInstance;
};
