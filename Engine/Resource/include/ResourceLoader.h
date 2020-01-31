#pragma once
#include "RenderHeader.h"


#define ResourceLoader() CResourceLoader::GetInstance()


class CALL_API CSceneNode;
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
	virtual bool LoadTextureCubemap(const char* szFileName, CGfxTextureCubemap* pTextureCubemap, int baseLevel, int numLevels);
	virtual bool LoadTextureCubemapFace(const char* szFileName, GfxCubemapFace face, CGfxTextureCubemap* pTextureCubemap, int baseLevel, int numLevels);

public:
	virtual CSceneNode* LoadSceneMesh(const char* szFileName, CSceneNode* pParentSceneNode, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0, int numLevels = INT_MAX);


protected:
	static CResourceLoader* pInstance;
};
