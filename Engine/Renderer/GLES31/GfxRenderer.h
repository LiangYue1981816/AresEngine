#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <gl31.h>
#include <gli.hpp>
#include <eastl/string.h>
#include <eastl/vector.h>
#include <eastl/map.h>
#include <eastl/unordered_map.h>

#include "Engine.h"
#include "Utils.h"
#include "Define.h"
#include "SharedPtr.h"

#include "GfxGLM.h"
#include "GfxDefinition.h"

#include "GfxResource.h"
#include "GfxMesh.h"
#include "GfxProgram.h"
#include "GfxSampler.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubeMap.h"
#include "GfxMaterial.h"
#include "GfxFrameBuffer.h"

#include "GfxMeshManager.h"
#include "GfxProgramManager.h"
#include "GfxSamplerManager.h"
#include "GfxTextureManager.h"
#include "GfxMaterialManager.h"
#include "GfxFrameBufferManager.h"

#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"
#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"

#include "GfxCamera.h"
#include "GfxCameraManager.h"
#include "GfxCommandBuffer.h"

#include "GfxProfiler.h"


class CGfxRenderer
{
	friend class CEngine;
	friend class CGfxMesh;
	friend class CGfxMaterial;
	friend class CGfxTextureBase;
	friend class CGfxFrameBuffer;
	friend class CGfxCommandBindCamera;
	friend class CGfxCommandBindMaterial;
	friend class CGfxCommandBindInputTexture;


private:
	CGfxRenderer(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath);
	virtual ~CGfxRenderer(void);


public:
	const char* GetShaderFullPath(const char *szFileName, char *szFullPath) const;
	const char* GetTextureFullPath(const char *szFileName, char *szFullPath) const;
	const char* GetMaterialFullPath(const char *szFileName, char *szFullPath) const;
	const char* GetMeshFullPath(const char *szFileName, char *szFullPath) const;

	// Camera
public:
	CGfxCamera* GetCamera(const char *szName);

	// Internal Gfx Resources
private:
	CGfxProgram* CreateProgram(const char *szVertexFileName, const char *szFragmentFileName);
	CGfxSampler* CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode);

	// External Gfx Resources
public:
	CGfxFrameBufferPtr CreateFrameBuffer(GLuint width, GLuint height, bool bDepthRenderBuffer);
	CGfxTexture2DPtr CreateTexture2D(GLuint name);
	CGfxTexture2DArrayPtr CreateTexture2DArray(GLuint name);
	CGfxTextureCubeMapPtr CreateTextureCubeMap(GLuint name);

	CGfxMeshPtr LoadMesh(const char *szFileName);
	CGfxMaterialPtr LoadMaterial(const char *szFileName);
	CGfxTexture2DPtr LoadTexture2D(const char *szFileName);
	CGfxTexture2DArrayPtr LoadTexture2DArray(const char *szFileName);
	CGfxTextureCubeMapPtr LoadTextureCubeMap(const char *szFileName);

private:
	void FreeMesh(CGfxMesh *pMesh);
	void FreeMaterial(CGfxMaterial *pMaterial);
	void FreeTexture(CGfxTextureBase *pTexture);
	void FreeFrameBuffer(CGfxFrameBuffer *pFrameBuffer);

	// Features
public:
	void SetTime(float t, float dt);

	void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	void SetShadowDistance(float distance);
	void SetShadowResolution(float resolution);

	void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor);

	void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);

	void SetPointLightColor(float red, float green, float blue);
	void SetPointLightPosition(float posx, float posy, float posz, float radius);
	void SetPointLightAttenuation(float linear, float square, float constant);

	void SetDirectLightColor(float red, float green, float blue);
	void SetDirectLightDirection(float dirx, float diry, float dirz);

	void SetFogColor(float red, float green, float blue);
	void SetFogHeightDensity(float startHeight, float endHeight, float density);
	void SetFogDistanceDensity(float startDistance, float endDistance, float density);

	// Gfx Commands
public:
	bool CmdBeginPass(CGfxCommandBuffer *pCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer);
	bool CmdEndPass(CGfxCommandBuffer *pCommandBuffer);

	bool CmdSetScissor(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);
	bool CmdSetCullFace(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum cullFace, GLenum frontFace);
	bool CmdSetDepthTest(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum depthFunc);
	bool CmdSetDepthWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnable);
	bool CmdSetColorWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	bool CmdSetBlend(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum srcFactor, GLenum dstFactor);
	bool CmdSetPolygonOffset(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLfloat factor, GLfloat units);

	bool CmdBindCamera(CGfxCommandBuffer *pCommandBuffer, CGfxCamera *pCamera);
	bool CmdBindMaterial(CGfxCommandBuffer *pCommandBuffer, const CGfxMaterialPtr &ptrMaterial);
	bool CmdBindInputTexture(CGfxCommandBuffer *pCommandBuffer, const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode);

	bool CmdClearDepth(CGfxCommandBuffer *pCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBuffer *pCommandBuffer, float red, float green, float blue, float alpha);
	bool CmdDrawInstance(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, int indexCount, int baseIndex, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdDrawIndirect(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, int indexCount, int baseIndex, GLsizei baseVertex, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdDrawScreen(CGfxCommandBuffer *pCommandBuffer);

	bool CmdExecute(CGfxCommandBuffer *pCommandBuffer, CGfxCommandBuffer *pSecondaryCommandBuffer);

	// Update & Render
public:
	void Update(void);
	void Submit(const CGfxCommandBuffer *pCommandBuffer);
	void Present(void);

private:
	void BindCamera(CGfxCamera *pCamera);
	void BindMaterial(CGfxMaterial *pMaterial);
	void BindInputTexture(const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode);


private:
	char m_szShaderPath[260];
	char m_szTexturePath[260];
	char m_szMaterialPath[260];
	char m_szMeshPath[260];

private:
	void *m_hDC;

private:
	CGfxMesh *m_pScreenMesh;
	CGfxMaterial *m_pGlobalMaterial;
	CGfxMaterial *m_pCurrentMaterial;

private:
	CGfxUniformEngine *m_pUniformEngine;

private:
	CGfxCameraManager *m_pCameraManager;
	CGfxFrameBufferManager *m_pFrameBufferManager;
	CGfxProgramManager *m_pProgramManager;
	CGfxSamplerManager *m_pSamplerManager;
	CGfxTextureManager *m_pTextureManager;
	CGfxMaterialManager *m_pMaterialManager;
	CGfxMeshManager *m_pMeshManager;
};
