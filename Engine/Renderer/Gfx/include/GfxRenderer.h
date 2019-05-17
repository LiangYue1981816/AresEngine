#pragma once
#include "PreHeader.h"

#include "GfxGLM.h"
#include "GfxEnum.h"
#include "GfxHelper.h"
#include "GfxDefinition.h"

#include "GfxSprivCross.h"
#include "GfxVertexAttribute.h"

#include "GfxProfiler.h"

#include "GfxResource.h"
#include "GfxResourceManager.h"

#include "GfxCamera.h"
#include "GfxRenderQueue.h"

#include "GfxSwapChain.h"
#include "GfxRenderPass.h"
#include "GfxFrameBuffer.h"

#include "GfxShader.h"
#include "GfxPipelineCompute.h"
#include "GfxPipelineGraphics.h"

#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxIndirectBuffer.h"

#include "GfxSampler.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubemap.h"
#include "GfxRenderTexture.h"

#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformEngine.h"
#include "GfxUniformCamera.h"
#include "GfxUniformBuffer.h"

#include "GfxMesh.h"
#include "GfxMeshDraw.h"

#include "GfxMaterial.h"
#include "GfxMaterialPass.h"

#include "GfxDescriptorSet.h"
#include "GfxDescriptorLayout.h"

#include "GfxCommandBuffer.h"


#define GfxRenderer() CGfxRenderer::GetInstance()


#define DESCRIPTOR_SET_ENGINE               0
#define DESCRIPTOR_SET_CAMERA               1
#define DESCRIPTOR_SET_PASS                 2
#define DESCRIPTOR_SET_INPUTATTACHMENT      3
#define DESCRIPTOR_SET_COUNT                4

#define DESCRIPTOR_BIND_ENGINE              0
#define DESCRIPTOR_BIND_CAMERA              1

#define UNIFORM_ENGINE_NAME                 HashValue("Engine")
#define UNIFORM_CAMERA_NAME                 HashValue("Camera")


class CALL_API CGfxRenderer
{
public:
	static CGfxRenderer* GetInstance(void);


public:
	CGfxRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CGfxRenderer(void);


public:
	void SetShaderCachePath(const char* szPath) const;
	void SetShaderIncludePath(const char* szPath) const;


public:
	virtual uint32_t GetLastError(void) const = 0;

public:
	virtual CGfxSwapChain* GetSwapChain(void) const = 0;

public:
	virtual bool IsSupportExtension(const char* extension) const = 0;

public:
	virtual CGfxShader* CreateShader(const char* szFileName, shader_kind kind) = 0;
	virtual CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader* pComputeShader) = 0;
	virtual CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding = 0, int instanceBinding = 1) = 0;
	virtual CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;

public:
	virtual CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments) = 0;

	virtual CGfxRenderPassPtr GetRenderPass(uint32_t name) = 0;
	virtual CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses) = 0;

	virtual CGfxTexture2DPtr GetTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxTextureCubemapPtr GetTextureCubemap(uint32_t name) = 0;
	virtual CGfxTextureCubemapPtr NewTextureCubemap(uint32_t name) = 0;
	virtual CGfxTextureCubemapPtr NewTextureCubemap(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxRenderTexturePtr GetRenderTexture(uint32_t name) = 0;
	virtual CGfxRenderTexturePtr NewRenderTexture(uint32_t name) = 0;

	virtual CGfxUniformBufferPtr NewUniformBuffer(size_t size) = 0;

	virtual CGfxMeshPtr GetMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding = 0) = 0;

	virtual CGfxMeshDrawPtr GetMeshDraw(uint32_t name) = 0;
	virtual CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding = 1) = 0;

	virtual CGfxMaterialPtr GetMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set) = 0;
	virtual CGfxDescriptorSetPtr NewDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout) = 0;
	virtual CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass) = 0;

	virtual CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer) = 0;

public:
	virtual bool CmdBeginRecord(CGfxCommandBufferPtr ptrCommandBuffer) = 0;
	virtual bool CmdEndRecord(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual bool CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass) = 0;
	virtual bool CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer) = 0;
	virtual bool CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual bool CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute) = 0;
	virtual bool CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics) = 0;
	virtual bool CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet) = 0;

	virtual bool CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0) = 0;
	virtual bool CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1) = 0;
	virtual bool CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2) = 0;
	virtual bool CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) = 0;
	virtual bool CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0) = 0;
	virtual bool CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1) = 0;
	virtual bool CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2) = 0;
	virtual bool CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual bool CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;

	virtual bool CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height) = 0;
	virtual bool CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height) = 0;

	virtual bool CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth) = 0;
	virtual bool CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha) = 0;

	virtual bool CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size) = 0;
	virtual bool CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size) = 0;

	virtual bool CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer) = 0;
	virtual bool CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual bool CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage) = 0;
	virtual bool CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual void Submit(const eastl::vector<CGfxCommandBufferPtr>& ptrCommandBuffers) = 0;

public:
	virtual void AcquireNextFrame(void) = 0;
	virtual void Present(void) = 0;


protected:
	static CGfxRenderer* pInstance;
};
