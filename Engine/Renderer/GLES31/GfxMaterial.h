#pragma once
#include <tinyxml.h>
#include <tinystr.h>
#include "GfxRenderer.h"


typedef struct GL_STATE {
	GLboolean bEnableCullFace;
	GLboolean bEnableDepthTest;
	GLboolean bEnableDepthWrite;
	GLboolean bEnableColorWrite[4];
	GLboolean bEnableBlend;
	GLboolean bEnablePolygonOffset;
	GLenum cullFace;
	GLenum frontFace;
	GLenum depthFunc;
	GLenum srcBlendFactor;
	GLenum dstBlendFactor;
	GLfloat polygonOffsetFactor;
	GLfloat polygonOffsetUnits;
} GL_STATE;


class CGfxMaterial : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxMaterialManager;


private:
	CGfxMaterial(GLuint name);
	virtual ~CGfxMaterial(void);
	virtual void Release(void);

	
private:
	bool Load(const char *szFileName);
	bool LoadState(TiXmlNode *pParentNode);
	bool LoadProgram(TiXmlNode *pParentNode);
	bool LoadTexture2D(TiXmlNode *pParentNode);
	bool LoadTexture2DArray(TiXmlNode *pParentNode);
	bool LoadTextureCubeMap(TiXmlNode *pParentNode);
	bool LoadUniformVec1(TiXmlNode *pParentNode);
	bool LoadUniformVec2(TiXmlNode *pParentNode);
	bool LoadUniformVec3(TiXmlNode *pParentNode);
	bool LoadUniformVec4(TiXmlNode *pParentNode);
	void Free(void);

public:
	void SetEnableCullFace(bool bEnable, GLenum cullFace, GLenum frontFace);
	void SetEnableDepthTest(bool bEnable, GLenum depthFunc);
	void SetEnableDepthWrite(bool bEnable);
	void SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	void SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor);
	void SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units);

	bool IsEnableBlend(void) const;

public:
	GLuint GetName(void) const;

private:
	CGfxProgram* GetProgram(void);
	CGfxSampler* GetSampler(const char *szName, GLenum minFilter, GLenum magFilter, GLenum addressMode);

public:
	GLuint GetTextureUnits(void) const;

	CGfxTexture2DPtr GetTexture2D(const char *szName);
	CGfxTexture2DArrayPtr GetTexture2DArray(const char *szName);
	CGfxTextureCubeMapPtr GetTextureCubeMap(const char *szName);

	CGfxUniformVec1* GetUniformVec1(const char *szName);
	CGfxUniformVec2* GetUniformVec2(const char *szName);
	CGfxUniformVec3* GetUniformVec3(const char *szName);
	CGfxUniformVec4* GetUniformVec4(const char *szName);
	CGfxUniformMat4* GetUniformMat4(const char *szName);

private:
	void Bind(void) const;
	void BindState(void) const;
	void BindUniforms(CGfxProgram *pProgram) const;
	void BindTextures(CGfxProgram *pProgram, GLuint indexUnit) const;


private:
	GLuint m_name;

private:
	GL_STATE m_state;
	CGfxProgram *m_pProgram;

	eastl::unordered_map<GLuint, CGfxSampler*> m_pSamplers;
	eastl::unordered_map<GLuint, CGfxTexture2DPtr> m_ptrTexture2ds;
	eastl::unordered_map<GLuint, CGfxTexture2DArrayPtr> m_ptrTexture2dArrays;
	eastl::unordered_map<GLuint, CGfxTextureCubeMapPtr> m_ptrTextureCubeMaps;

	eastl::unordered_map<GLuint, CGfxUniformVec1*> m_pUniformVec1s;
	eastl::unordered_map<GLuint, CGfxUniformVec2*> m_pUniformVec2s;
	eastl::unordered_map<GLuint, CGfxUniformVec3*> m_pUniformVec3s;
	eastl::unordered_map<GLuint, CGfxUniformVec4*> m_pUniformVec4s;
	eastl::unordered_map<GLuint, CGfxUniformMat4*> m_pUniformMat4s;
};
