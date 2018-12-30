#pragma once
#include "PreHeader.h"

#include "GfxProfiler.h"

#include "GfxGLM.h"
#include "GfxEnum.h"
#include "GfxDefinition.h"
#include "GfxVertexAttribute.h"

#include "GfxResource.h"

#include "GfxCamera.h"
#include "GfxRenderQueue.h"

#include "GfxSwapChain.h"
#include "GfxRenderPass.h"
#include "GfxFrameBuffer.h"

#include "GfxMesh.h"
#include "GfxMeshDraw.h"

#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxIndirectBuffer.h"

#include "GfxSampler.h"

#include "GfxTexture.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubeMap.h"

#include "GfxRenderTexture.h"

#include "GfxShader.h"
#include "GfxPipeline.h"
#include "GfxPipelineCompute.h"
#include "GfxPipelineGraphics.h"

#include "GfxMaterial.h"
#include "GfxMaterialPass.h"

#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"
#include "GfxUniformBuffer.h"

#include "GfxCommandBuffer.h"

#include "GfxResourceManager.h"


#define GfxRenderer() CGfxRenderer::GetInstance()


#define UNIFORM_ENGINE_NAME "Engine"
#define UNIFORM_CAMERA_NAME "Camera"


class CALL_API CGfxRenderer
{
public:
	static CGfxRenderer* GetInstance(void);


public:
	CGfxRenderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CGfxRenderer(void);


public:
	void SetShaderCachePath(const char *szPath) const;
	void SetShaderIncludePath(const char *szPath) const;


public:
	virtual uint32_t GetLastError(void) const = 0;

public:
	virtual CGfxSwapChain* GetSwapChain(void) const = 0;
	virtual CGfxMaterialPass* GetGlobalPass(void) const = 0;

public:
	virtual CGfxShader* CreateShader(const char *szFileName, shader_kind kind) = 0;
	virtual CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader) = 0;
	virtual CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state) = 0;
	virtual CGfxSampler* CreateSampler(GfxMinFilter minFilter, GfxMagFilter magFilter, GfxAddressMode addressMode) = 0;

public:
	virtual CGfxRenderPassPtr NewRenderPass(int numAttachments, int numSubpasses) = 0;
	virtual CGfxFrameBufferPtr NewFrameBuffer(int width, int height) = 0;

	virtual bool IsHaveMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(const char *szFileName, uint32_t vertexBinding = 0) = 0;

	virtual bool IsHaveMeshDraw(uint32_t name) = 0;
	virtual CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding = 1) = 0;

	virtual bool IsHaveMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(const char *szFileName) = 0;

	virtual bool IsHaveTexture2D(uint32_t name) = 0;
	virtual bool IsHaveTexture2DArray(uint32_t name) = 0;
	virtual bool IsHaveTextureCubeMap(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(const char *szFileName) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName) = 0;
	virtual CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name) = 0;
	virtual CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName) = 0;

	virtual bool IsHaveRenderTexture(uint32_t name) = 0;
	virtual CGfxRenderTexturePtr NewRenderTexture(uint32_t name) = 0;

	virtual CGfxUniformBufferPtr NewUniformBuffer(size_t size) = 0;
	virtual CGfxCommandBufferPtr NewCommandBuffer(bool bMainCommandBuffer) = 0;

public:
	virtual bool CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass) = 0;
	virtual bool CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;
	virtual bool CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

	virtual bool CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute) = 0;
	virtual bool CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics) = 0;
	virtual bool CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass) = 0;
	virtual bool CmdBindUniformBuffer(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform) = 0;

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

	virtual bool CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth) = 0;
	virtual bool CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha) = 0;

	virtual bool CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height) = 0;
	virtual bool CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height) = 0;

	virtual bool CmdSetInstanceBufferData(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size) = 0;

	virtual bool CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, int instanceCount) = 0;
	virtual bool CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw) = 0;
	virtual bool CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

	virtual bool CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer) = 0;

	virtual bool CmdPresent(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

	virtual bool CmdPushDebugGroup(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szMessage) = 0;
	virtual bool CmdPopDebugGroup(CGfxCommandBufferPtr &ptrCommandBuffer) = 0;

public:
	virtual void Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) = 0;
	virtual void Present(void) = 0;


protected:
	static CGfxRenderer *pInstance;
};
