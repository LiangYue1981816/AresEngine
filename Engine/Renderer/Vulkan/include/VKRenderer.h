#pragma once
#include "GfxHeader.h"

#include "Vulkan/Vulkan.h"
/*
#include "VKHelper.h"
#include "VKExtension.h"
#include "VKDefinition.h"

#include "VKAllocator.h"

#include "VKInstance.h"
#include "VKDevice.h"
#include "VKQueue.h"

#include "VKMemory.h"
#include "VKMemoryAllocator.h"
#include "VKMemoryManager.h"

#include "VKImage.h"

#include "VKSwapChain.h"
#include "VKRenderPass.h"
#include "VKRenderPassManager.h"
#include "VKFrameBuffer.h"
#include "VKFrameBufferManager.h"

#include "VKShader.h"
#include "VKShaderManager.h"
#include "VKPipeline.h"
#include "VKPipelineCompute.h"
#include "VKPipelineGraphics.h"
#include "VKPipelineManager.h"

#include "VKBuffer.h"
#include "VKIndexBuffer.h"
#include "VKVertexBuffer.h"
#include "VKInstanceBuffer.h"
#include "VKIndirectBuffer.h"

#include "VKSampler.h"
#include "VKSamplerManager.h"
#include "VKTexture.h"
#include "VKTexture2D.h"
#include "VKTexture2DArray.h"
#include "VKTextureCubeMap.h"
#include "VKTextureManager.h"
#include "VKRenderTexture.h"
#include "VKRenderTextureManager.h"

#include "VKUniformBuffer.h"
#include "VKUniformBufferManager.h"

#include "VKMesh.h"
#include "VKMeshManager.h"
#include "VKMeshDraw.h"
#include "VKMeshDrawManager.h"

#include "VKMaterial.h"
#include "VKMaterialPass.h"
#include "VKMaterialManager.h"

#include "VKDescriptorLayout.h"
#include "VKDescriptorPool.h"
#include "VKDescriptorSet.h"
#include "VKDescriptorSetManager.h"

#include "VKCommandPool.h"
#include "VKCommandBuffer.h"
#include "VKCommandBufferManager.h"
*/

#define VKRenderer() ((CVKRenderer *)CGfxRenderer::GetInstance())


class CALL_API CVKRenderer : public CGfxRenderer
{
public:
	CVKRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CVKRenderer(void);


public:
	uint32_t GetLastError(void) const = 0;

public:
	CGfxSwapChain* GetSwapChain(void) const = 0;

public:
	bool IsSupportExtension(const char* extension) const = 0;

public:
	CGfxShader* CreateShader(const char* szFileName, shader_kind kind) = 0;
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader* pComputeShader) = 0;
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1) = 0;
	CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;

public:
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments) = 0;

	CGfxRenderPassPtr GetRenderPass(uint32_t name) = 0;
	CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses) = 0;

	CGfxTexture2DPtr GetTexture2D(uint32_t name) = 0;
	CGfxTexture2DPtr NewTexture2D(uint32_t name) = 0;
	CGfxTexture2DPtr NewTexture2D(const char* szFileName, uint32_t baseLevel = 0) = 0;

	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) = 0;
	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name) = 0;
	CGfxTexture2DArrayPtr NewTexture2DArray(const char* szFileName, uint32_t baseLevel = 0) = 0;

	CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name) = 0;
	CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name) = 0;
	CGfxTextureCubeMapPtr NewTextureCubeMap(const char* szFileName, uint32_t baseLevel = 0) = 0;

	CGfxRenderTexturePtr GetRenderTexture(uint32_t name) = 0;
	CGfxRenderTexturePtr NewRenderTexture(uint32_t name) = 0;

	CGfxUniformBufferPtr NewUniformBuffer(size_t size) = 0;

	CGfxMeshPtr GetMesh(uint32_t name) = 0;
	CGfxMeshPtr NewMesh(uint32_t name) = 0;
	CGfxMeshPtr NewMesh(const char* szFileName, uint32_t vertexBinding = 0) = 0;

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name) = 0;
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, uint32_t instanceBinding = 1) = 0;

	CGfxMaterialPtr GetMaterial(uint32_t name) = 0;
	CGfxMaterialPtr NewMaterial(uint32_t name) = 0;
	CGfxMaterialPtr NewMaterial(const char* szFileName, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1, uint32_t baseLevel = 0) = 0;

	CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set) = 0;
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout) = 0;
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass) = 0;

	CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer) = 0;

public:
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

public:
	virtual void Submit(const eastl::vector<CGfxCommandBufferPtr>& ptrCommandBuffers) = 0;
	virtual void AcquireNextFrame(void) = 0;
	virtual void Present(void) = 0;
};
