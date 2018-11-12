#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"

#include <gles3/gl31.h>
#include <gli/gli.hpp>
#include <shaderc/shaderc.hpp>
#include <glslc/file_includer.h>
#include <spirv-cross/spirv_cfg.hpp>
#include <spirv-cross/spirv_msl.hpp>
#include <spirv-cross/spirv_glsl.hpp>

#include "Define.h"
#include "Utils.h"
#include "Allocator.h"
#include "SharedPtr.h"
#include "ShaderCompiler.h"
#include "FileManager.h"

#include "GfxGLM.h"
#include "GfxDefinition.h"
#include "GfxVertexAttribute.h"

#include "GfxProfiler.h"
#include "GfxResource.h"

#include "GfxCamera.h"
#include "GfxRenderQueue.h"

#include "GfxSwapChain.h"
#include "GfxRenderPass.h"
#include "GfxFrameBuffer.h"

#include "GfxMesh.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxDrawIndirectBuffer.h"

#include "GfxSampler.h"
#include "GfxTextureBase.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubeMap.h"

#include "GfxShader.h"
#include "GfxPipelineBase.h"
#include "GfxPipelineCompute.h"
#include "GfxPipelineGraphics.h"

#include "GfxMaterial.h"
#include "GfxMaterialPass.h"

#include "GfxUniformBase.h"
#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"
#include "GfxUniformBuffer.h"

#include "GfxCommandBuffer.h"


#define RendererCreate(api, hDC, width, height, format) CGfxRenderer::Create((api), (hDC), (width), (height), (format))
#define RendererDestroy() CGfxRenderer::Destroy()
#define Renderer() CGfxRenderer::GetInstance()


#define UNIFORM_ENGINE_NAME "Engine"
#define UNIFORM_CAMERA_NAME "Camera"


typedef enum GfxApi {
	GFX_API_GLES3 = 0,
	GFX_API_VULKAN,
	GFX_API_METAL
} GfxApi;


class CALL_API CGfxRenderer
{
public:
	static CGfxRenderer* GetInstance(void);
	static void Create(GfxApi api, void *hDC, int width, int height, uint32_t format);
	static void Destroy(void);


protected:
	CGfxRenderer(void *hDC, int width, int height, uint32_t format);
	virtual ~CGfxRenderer(void);


public:
	void SetShaderCachePath(const char *szPath) const;
	void SetShaderIncludePath(const char *szPath) const;


public:
	virtual CGfxSwapChain* GetSwapChain(void) const = 0;

public:
	virtual CGfxRenderPassPtr NewRenderPass(int numAttachments, int numSubpasses) = 0;
	virtual CGfxFrameBufferPtr NewFrameBuffer(int width, int height) = 0;

	virtual CGfxMaterialPtr NewMaterial(const char *szFileName) = 0;

	virtual CGfxMeshPtr NewMesh(uint32_t name, uint32_t instanceFormat) = 0;
	virtual CGfxMeshPtr NewMesh(const char *szFileName, uint32_t instanceFormat) = 0;

	virtual CGfxTexture2DPtr NewTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(const char *szFileName) = 0;

	virtual CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName) = 0;

	virtual CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name) = 0;
	virtual CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName) = 0;

	virtual CGfxUniformEnginePtr NewUniformEngine(bool bDynamic) = 0;
	virtual CGfxUniformCameraPtr NewUniformCamera(bool bDynamic) = 0;

	virtual CGfxCommandBufferPtr NewCommandBuffer(bool bMainCommandBuffer) = 0;

public:
	virtual bool CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass) = 0;
	virtual bool CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;
	virtual bool CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

	virtual bool CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute) = 0;
	virtual bool CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics) = 0;
	virtual bool CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass) = 0;
	virtual bool CmdBindUniformEngine(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine) = 0;
	virtual bool CmdBindUniformCamera(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformCameraPtr &ptrUniformCamera) = 0;

	virtual bool CmdUniform1i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0) = 0;
	virtual bool CmdUniform2i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1) = 0;
	virtual bool CmdUniform3i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2) = 0;
	virtual bool CmdUniform4i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3) = 0;
	virtual bool CmdUniform1f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0) = 0;
	virtual bool CmdUniform2f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1) = 0;
	virtual bool CmdUniform3f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2) = 0;
	virtual bool CmdUniform4f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3) = 0;
	virtual bool CmdUniform1iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform2iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform3iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform4iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform1fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value) = 0;

	virtual bool CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height) = 0;
	virtual bool CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height) = 0;

	virtual bool CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth) = 0;
	virtual bool CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha) = 0;

	virtual bool CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size) = 0;
	virtual bool CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size) = 0;
	virtual bool CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

	virtual bool CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer) = 0;

public:
	virtual void Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) = 0;
	virtual void Present(void) = 0;


protected:
	static CGfxRenderer *pInstance;
};
