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
#include "VKRenderTexture.h"
#include "VKRenderTextureManager.h"
#include "VKFrameBuffer.h"
#include "VKFrameBufferManager.h"

#include "VKShader.h"
#include "VKShaderManager.h"

#include "VKPipeline.h"
#include "VKPipelineCompute.h"
#include "VKPipelineGraphics.h"
#include "VKPipelineManager.h"

#include "VKTransferBuffer.h"
#include "VKTransferManager.h"

#include "VKBuffer.h"
#include "VKIndexBuffer.h"
#include "VKVertexBuffer.h"
#include "VKIndirectBuffer.h"

#include "VKInstanceBuffer.h"
#include "VKInstanceBufferManager.h"

#include "VKUniformBuffer.h"
#include "VKUniformBufferManager.h"

#include "VKStorageBuffer.h"
#include "VKStorageBufferManager.h"

#include "VKSampler.h"
#include "VKSamplerManager.h"
#include "VKTexture.h"
#include "VKTexture2D.h"
#include "VKTexture2DArray.h"
#include "VKTextureCubemap.h"
#include "VKTextureManager.h"

#include "VKMesh.h"
#include "VKMeshManager.h"
#include "VKMeshDraw.h"
#include "VKMeshDrawManager.h"

#include "VKMaterial.h"
#include "VKMaterialPass.h"
#include "VKMaterialManager.h"

#include "VKDescriptorPool.h"
#include "VKDescriptorSet.h"
#include "VKDescriptorSetManager.h"
#include "VKDescriptorLayout.h"
#include "VKDescriptorLayoutManager.h"

#include "VKSemaphore.h"
#include "VKCommandBuffer.h"
#include "VKCommandBufferManager.h"


#define VKRenderer() ((CVKRenderer *)CGfxRenderer::GetInstance())


class CVKRenderer : public CGfxRenderer
{
public:
	CVKRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CVKRenderer(void);


public:
	GfxApi GetAPI(void) const;
	const glm::mat4& GetBaseMatrix(void) const;


public:
	uint32_t GetLastError(void) const;

public:
	CGfxSwapChain* GetSwapChain(void) const;

public:
	bool IsSupportExtension(const char* extension) const;

public:
	CGfxShader* CreateShader(const char* szFileName, shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader* pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
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
	CGfxStorageBufferPtr NewStorageBuffer(size_t size);

	CGfxInstanceBufferPtr NewInstanceBuffer(uint32_t instanceFormat, int instanceBinding);
	CGfxMultiInstanceBufferPtr NewMultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count);

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom);
	CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set);

	CGfxDescriptorSetPtr GetDescriptorSet(uint32_t name);
	CGfxDescriptorSetPtr NewDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CGfxDescriptorSetPtr NewDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom);
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);

	CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer);

public:
	void BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer);
	void BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass);
	void EndRecord(CGfxCommandBufferPtr ptrCommandBuffer);

	// Outside RenderPass
	void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetBufferBarrier(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags);

	// Inside RenderPass
	void CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	void CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer);
	void CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer);

	void CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute);
	void CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics);
	void CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet);
	void CmdBindIndexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer);
	void CmdBindVertexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer);
	void CmdBindInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset);

	void CmdSetIndexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer, const uint8_t* pBuffer, uint32_t size);
	void CmdSetVertexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer, const uint8_t* pBuffer, uint32_t size);
	void CmdSetInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer, const uint8_t* pBuffer, uint32_t size);

	void CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0);
	void CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1);
	void CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2);
	void CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3);
	void CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0);
	void CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1);
	void CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2);
	void CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3);
	void CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	void CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	void CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	void CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value);
	void CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);
	void CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value);

	void CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	void CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);

	void CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth);
	void CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha);

	void CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, int indexType, int indexOffset, int indexCount, int instanceCount);
	void CmdDispatch(CGfxCommandBufferPtr ptrCommandBuffer, int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ);

	void CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage);
	void CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer);

	void CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer);

	// Submit
	void Submit(const CGfxCommandBufferPtr& ptrCommandBuffer, const CGfxSemaphore* pWaitSemaphore);

public:
	void AcquireNextFrame(void);
	void Present(const CGfxSemaphore* pWaitSemaphore);


private:
	CVKInstance* m_pInstance;
	CVKDevice* m_pDevice;
	CVKSwapChain* m_pSwapChain;

private:
	CVKShaderManager* m_pShaderManager;
	CVKPipelineComputeManager* m_pPipelineComputeManager;
	CVKPipelineGraphicsManager* m_pPipelineGraphicsManager;
	CVKDescriptorLayoutManager* m_pDescriptorLayoutManager;
	CVKSamplerManager* m_pSamplerManager;
	CVKTexture2DManager* m_pTexture2DManager;
	CVKTexture2DArrayManager* m_pTexture2DArrayManager;
	CVKTextureCubemapManager* m_pTextureCubemapManager;
	CVKUniformBufferManager* m_pUniformBufferManager;
	CVKStorageBufferManager* m_pStorageBufferManager;
	CVKInstanceBufferManager* m_pInstanceBufferManager;
	CVKMeshManager* m_pMeshManager;
	CVKMeshDrawManager* m_pMeshDrawManager;
	CVKMaterialManager* m_pMaterialManager;
	CVKRenderPassManager* m_pRenderPassManager;
	CVKRenderTextureManager* m_pRenderTextureManager;
	CVKFrameBufferManager* m_pFrameBufferManager;
	CVKDescriptorSetManager* m_pDescriptorSetManager;
	CVKCommandBufferManager* m_pCommandBufferManager;
};
