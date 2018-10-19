#pragma once
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

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
#include "GfxRenderPass.h"
#include "GfxFrameBuffer.h"

#include "GfxResourceManager.h"
#include "GfxMeshManager.h"
#include "GfxShaderManager.h"
#include "GfxSamplerManager.h"
#include "GfxTextureManager.h"
#include "GfxPipelineManager.h"
#include "GfxMaterialManager.h"
#include "GfxRenderPassManager.h"
#include "GfxFrameBufferManager.h"

#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"

#include "GfxCamera.h"
#include "GfxSwapChain.h"
#include "GfxRenderQueue.h"
#include "GfxCommandBuffer.h"
#include "GfxShaderCompiler.h"

#include "GfxProfiler.h"


class CGfxRenderer
{
	friend class CEngine;
	friend class CGfxMesh;
	friend class CGfxMaterial;
	friend class CGfxMaterialPass;
	friend class CGfxTextureBase;
	friend class CGfxRenderPass;
	friend class CGfxFrameBuffer;
	friend class CGfxCommandBindCamera;
	friend class CGfxCommandBindPipeline;
	friend class CGfxCommandBindMaterialPass;
	friend class CGfxCommandBindInputTexture;
	friend class CGfxCommandBindSubPassInputTexture;
	friend class CGfxCommandUniform1f;
	friend class CGfxCommandUniform2f;
	friend class CGfxCommandUniform3f;
	friend class CGfxCommandUniform4f;
	friend class CGfxCommandUniform1fv;
	friend class CGfxCommandUniform2fv;
	friend class CGfxCommandUniform3fv;
	friend class CGfxCommandUniform4fv;
	friend class CGfxCommandUniformMatrix2fv;
	friend class CGfxCommandUniformMatrix3fv;
	friend class CGfxCommandUniformMatrix4fv;


public:
	static CGfxRenderer* GetInstance(void);


private:
	CGfxRenderer(void *hDC, const char *szShaderCachePath, int width, int height, uint32_t format);
	virtual ~CGfxRenderer(void);


#pragma region SwapChain
public:
	CGfxSwapChain* GetSwapChain(void) const;
#pragma endregion

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
	CGfxRenderPassPtr CreateRenderPass(int numAttachments, int numSubpasses);
	CGfxFrameBufferPtr CreateFrameBuffer(int width, int height);

	CGfxMeshPtr LoadMesh(const char *szFileName);
	CGfxMaterialPtr LoadMaterial(const char *szFileName);
	CGfxTexture2DPtr LoadTexture2D(const char *szFileName);
	CGfxTexture2DArrayPtr LoadTexture2DArray(const char *szFileName);
	CGfxTextureCubeMapPtr LoadTextureCubeMap(const char *szFileName);

private:
	void DestroyMesh(CGfxMesh *pMesh);
	void DestroyMaterial(CGfxMaterial *pMaterial);
	void DestroyTexture(CGfxTextureBase *pTexture);
	void DestroyRenderPass(CGfxRenderPass *pRenderPass);
	void DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer);
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
	bool CmdBeginRenderPass(CGfxCommandBuffer *pCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
	bool CmdEndRenderPass(CGfxCommandBuffer *pCommandBuffer);

	bool CmdSetScissor(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height);

	bool CmdBindCamera(CGfxCommandBuffer *pCommandBuffer, CGfxCamera *pCamera);
	bool CmdBindPipeline(CGfxCommandBuffer *pCommandBuffer, CGfxPipelineBase *pPipeline);
	bool CmdBindMaterialPass(CGfxCommandBuffer *pCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindInputTexture(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

	bool CmdUniform1f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0);
	bool CmdUniform2f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniform2fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniform3fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniform4fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix2fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix3fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix4fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value);

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

#pragma region Uniform
private:
	void Uniform1f(uint32_t name, float v0) const;
	void Uniform2f(uint32_t name, float v0, float v1) const;
	void Uniform3f(uint32_t name, float v0, float v1, float v2) const;
	void Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const;
	void Uniform1fv(uint32_t name, uint32_t count, const float *value) const;
	void Uniform2fv(uint32_t name, uint32_t count, const float *value) const;
	void Uniform3fv(uint32_t name, uint32_t count, const float *value) const;
	void Uniform4fv(uint32_t name, uint32_t count, const float *value) const;
	void UniformMatrix2fv(uint32_t name, uint32_t count, const float *value) const;
	void UniformMatrix3fv(uint32_t name, uint32_t count, const float *value) const;
	void UniformMatrix4fv(uint32_t name, uint32_t count, const float *value) const;
#pragma endregion


private:
	CGfxSwapChain *m_pSwapChain;

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
	CGfxRenderPassManager *m_pRenderPassManager;
	CGfxFrameBufferManager *m_pFrameBufferManager;

private:
	CGfxShaderCompiler *m_pShaderCompiler;

private:
	static CGfxRenderer *pInstance;
};
