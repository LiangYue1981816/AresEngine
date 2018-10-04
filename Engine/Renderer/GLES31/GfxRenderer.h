#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <gl31.h>
#include <gli.hpp>
#include <spirv_cfg.hpp>
#include <spirv_glsl.hpp>
#include <shaderc/shaderc.hpp>
#include <glslc/file_includer.h>

#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <eastl/string.h>
#include <eastl/map.h>
#include <eastl/vector.h>
#include <eastl/unordered_map.h>

#include "Engine.h"
#include "Utils.h"
#include "Define.h"
#include "SharedPtr.h"

#include "GfxGLM.h"
#include "GfxExtension.h"
#include "GfxDefinition.h"

#include "GfxResource.h"
#include "GfxResourceManager.h"

#include "GfxMesh.h"
#include "GfxShader.h"
#include "GfxSampler.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubeMap.h"
#include "GfxPipelineCompute.h"
#include "GfxPipelineGraphics.h"
#include "GfxMaterial.h"
#include "GfxMaterialPass.h"
#include "GfxFrameBuffer.h"

#include "GfxMeshManager.h"
#include "GfxShaderManager.h"
#include "GfxSamplerManager.h"
#include "GfxTextureManager.h"
#include "GfxPipelineManager.h"
#include "GfxMaterialManager.h"
#include "GfxFrameBufferManager.h"

#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"

#include "GfxShaderCompiler.h"
#include "GfxCommandBuffer.h"
#include "GfxCamera.h"

#include "GfxProfiler.h"


class CGfxRenderer
{
	friend class CEngine;
	friend class CGfxFrameBuffer;
	friend class CGfxMesh;
	friend class CGfxTextureBase;
	friend class CGfxMaterial;
	friend class CGfxMaterialPass;
	friend class CGfxCommandBindCamera;
	friend class CGfxCommandBindPipeline;
	friend class CGfxCommandBindMaterialPass;
	friend class CGfxCommandBindInputTexture;


private:
	CGfxRenderer(void *hDC, const char *szShaderCachePath);
	virtual ~CGfxRenderer(void);


#pragma region Shader Compiler
public:
	CGfxShaderCompiler* GetShaderCompiler(void) const;
#pragma endregion

#pragma region Resource Path
public:
	void SetResourcePath(const char *szPathName, const char *szExtName);
	const char* GetResourceFullName(const char *szFileName) const;
#pragma endregion

#pragma region Internal Gfx Resources
private:
	CGfxShader* LoadShader(const char *szFileName, shaderc_shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state);
	CGfxSampler* CreateSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);
#pragma endregion

#pragma region External Gfx Resources
public:
	CGfxTexture2DPtr CreateTexture2D(uint32_t name);
	CGfxTexture2DArrayPtr CreateTexture2DArray(uint32_t name);
	CGfxTextureCubeMapPtr CreateTextureCubeMap(uint32_t name);
	CGfxFrameBufferPtr CreateFrameBuffer(uint32_t width, uint32_t height, bool bDepthRenderBuffer);

	CGfxMeshPtr LoadMesh(const char *szFileName);
	CGfxMaterialPtr LoadMaterial(const char *szFileName);
	CGfxTexture2DPtr LoadTexture2D(const char *szFileName);
	CGfxTexture2DArrayPtr LoadTexture2DArray(const char *szFileName);
	CGfxTextureCubeMapPtr LoadTextureCubeMap(const char *szFileName);

private:
	void DestroyMesh(CGfxMesh *pMesh);
	void DestroyMaterial(CGfxMaterial *pMaterial);
	void DestroyTexture(CGfxTextureBase *pTexture);
	void DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer);
#pragma endregion

#pragma region Camera
public:
	CGfxCamera* CreateCamera(void) const;
	void DestroyCamera(CGfxCamera *pCamera) const;
#pragma endregion

#pragma region Features
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
#pragma endregion

#pragma region Commands
public:
	bool CmdBeginRenderPass(CGfxCommandBuffer *pCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer);
	bool CmdEndRenderPass(CGfxCommandBuffer *pCommandBuffer);

	bool CmdSetScissor(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);
	bool CmdSetCullFace(CGfxCommandBuffer *pCommandBuffer, bool bEnable, uint32_t cullFace, uint32_t frontFace);
	bool CmdSetDepthTest(CGfxCommandBuffer *pCommandBuffer, bool bEnable, uint32_t depthFunc);
	bool CmdSetDepthWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnable);
	bool CmdSetColorWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	bool CmdSetBlend(CGfxCommandBuffer *pCommandBuffer, bool bEnable, uint32_t srcFactor, uint32_t dstFactor);
	bool CmdSetPolygonOffset(CGfxCommandBuffer *pCommandBuffer, bool bEnable, float factor, float units);

	bool CmdBindCamera(CGfxCommandBuffer *pCommandBuffer, CGfxCamera *pCamera);
	bool CmdBindPipeline(CGfxCommandBuffer *pCommandBuffer, CGfxPipelineBase *pPipeline);
	bool CmdBindMaterialPass(CGfxCommandBuffer *pCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindInputTexture(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

	bool CmdClearDepth(CGfxCommandBuffer *pCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBuffer *pCommandBuffer, float red, float green, float blue, float alpha);
	bool CmdDrawInstance(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, int indexCount, int baseIndex, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdDrawIndirect(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, int indexCount, int baseIndex, int baseVertex, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdDrawScreen(CGfxCommandBuffer *pCommandBuffer);

	bool CmdExecute(CGfxCommandBuffer *pCommandBuffer, CGfxCommandBuffer *pSecondaryCommandBuffer);
#pragma endregion

#pragma region Draw
public:
	void Submit(const CGfxCommandBuffer *pCommandBuffer);
	void Present(void);
#pragma endregion

#pragma region Bind
private:
	void BindPipeline(CGfxPipelineBase *pPipeline);
	void BindCamera(CGfxCamera *pCamera);
	void BindMaterialPass(CGfxMaterialPass *pPass);
	void BindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);
#pragma endregion


private:
	void *m_hDC;

private:
	CGfxMesh *m_pScreenMesh;
	CGfxMaterialPass *m_pGlobalPass;
	CGfxMaterialPass *m_pCurrentPass;
	CGfxPipelineBase *m_pCurrentPipeline;

private:
	CGfxUniformEngine *m_pUniformEngine;

private:
	CGfxResourceManager *m_pResourceManager;
	CGfxMeshManager *m_pMeshManager;
	CGfxShaderManager *m_pShaderManager;
	CGfxSamplerManager *m_pSamplerManager;
	CGfxTextureManager *m_pTextureManager;
	CGfxPipelineManager *m_pPipelineManager;
	CGfxMaterialManager *m_pMaterialManager;
	CGfxFrameBufferManager *m_pFrameBufferManager;

private:
	CGfxShaderCompiler *m_pShaderCompiler;
};
