#pragma once
#include "GfxHeader.h"

#include "Vulkan/Vulkan.h"

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

#include "VKSwapChain.h"
#include "VKRenderPass.h"
#include "VKRenderPassManager.h"
#include "VKFrameBuffer.h"
#include "VKFrameBufferManager.h"

#include "VKShader.h"
#include "VKShaderManager.h"

#include "VKPipeline.h"

#include "VKTransferBuffer.h"
#include "VKTransferManager.h"

#include "VKBuffer.h"
#include "VKIndexBuffer.h"
#include "VKVertexBuffer.h"
#include "VKInstanceBuffer.h"
#include "VKIndirectBuffer.h"

#include "VKUniformBuffer.h"
#include "VKUniformBufferManager.h"

#include "VKSampler.h"
#include "VKSamplerManager.h"
#include "VKTexture.h"
#include "VKTexture2D.h"
#include "VKTexture2DArray.h"
#include "VKTextureCubemap.h"
#include "VKTextureManager.h"
#include "VKRenderTexture.h"
#include "VKRenderTextureManager.h"

#include "VKMesh.h"
#include "VKMeshManager.h"
#include "VKMeshDraw.h"
#include "VKMeshDrawManager.h"

#include "VKDescriptorLayout.h"
#include "VKDescriptorLayoutManager.h"

#include "VKDescriptorPool.h"
#include "VKDescriptorSet.h"
#include "VKDescriptorSetManager.h"

#include "VKCommandBuffer.h"
#include "VKCommandBufferManager.h"
/*
#include "VKPipelineCompute.h"
#include "VKPipelineGraphics.h"
#include "VKPipelineManager.h"

#include "VKMaterial.h"
#include "VKMaterialPass.h"
#include "VKMaterialManager.h"
*/

#define VKRenderer() ((CVKRenderer *)CGfxRenderer::GetInstance())


class CVKRenderer : public CGfxRenderer
{
public:
	CVKRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CVKRenderer(void);


public:
	uint32_t GetLastError(void) const;

public:
	CGfxSwapChain* GetSwapChain(void) const;

public:
	bool IsSupportExtension(const char* extension) const;

public:
	CGfxShader* CreateShader(const char* szFileName, shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader* pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding = 0, int instanceBinding = 1);
	CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);

public:
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments);

	CGfxRenderPassPtr GetRenderPass(uint32_t name);
	CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses);

	CGfxTexture2DPtr GetTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxTextureCubemapPtr GetTextureCubemap(uint32_t name);
	CGfxTextureCubemapPtr NewTextureCubemap(uint32_t name);
	CGfxTextureCubemapPtr NewTextureCubemap(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxRenderTexturePtr GetRenderTexture(uint32_t name);
	CGfxRenderTexturePtr NewRenderTexture(uint32_t name);

	CGfxUniformBufferPtr NewUniformBuffer(size_t size);

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding = 0);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding = 1);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set);
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);

	CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer);

public:
	bool CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	bool CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer);

	bool CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute);
	bool CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics);
	bool CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet);

	bool CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0);
	bool CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1);
	bool CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2);
	bool CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0);
	bool CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	bool CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	bool CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	bool CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	bool CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	bool CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);

	bool CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);

	bool CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha);

	bool CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size);
	bool CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size);

	bool CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer);
	bool CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer);

	bool CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage);
	bool CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer);

	void Submit(const eastl::vector<CGfxCommandBufferPtr>& ptrCommandBuffers);

public:
	void AcquireNextFrame(void);
	void Present(void);


private:
	CVKInstance* m_pInstance;
	CVKDevice* m_pDevice;
	CVKSwapChain* m_pSwapChain;
};
