#pragma once
#include "GfxHeader.h"

#include "GLES3/GL31.h"
#include "GLES3Helper.h"
#include "GLES3Extension.h"
#include "GLES3Definition.h"

#include "GLES3SwapChain.h"
#include "GLES3RenderPass.h"
#include "GLES3RenderPassManager.h"
#include "GLES3RenderTexture.h"
#include "GLES3RenderTextureManager.h"
#include "GLES3FrameBuffer.h"
#include "GLES3FrameBufferManager.h"

#include "GLES3Shader.h"
#include "GLES3ShaderManager.h"
#include "GLES3Pipeline.h"
#include "GLES3PipelineCompute.h"
#include "GLES3PipelineGraphics.h"
#include "GLES3PipelineManager.h"

#include "GLES3Buffer.h"
#include "GLES3IndexBuffer.h"
#include "GLES3VertexBuffer.h"
#include "GLES3IndirectBuffer.h"

#include "GLES3InstanceBuffer.h"
#include "GLES3InstanceBufferManager.h"

#include "GLES3UniformBuffer.h"
#include "GLES3UniformBufferManager.h"

#include "GLES3StorageBuffer.h"
#include "GLES3StorageBufferManager.h"

#include "GLES3Sampler.h"
#include "GLES3SamplerManager.h"
#include "GLES3Texture.h"
#include "GLES3Texture2D.h"
#include "GLES3Texture2DArray.h"
#include "GLES3TextureCubemap.h"
#include "GLES3TextureManager.h"

#include "GLES3Mesh.h"
#include "GLES3MeshManager.h"
#include "GLES3MeshDraw.h"
#include "GLES3MeshDrawManager.h"

#include "GLES3Material.h"
#include "GLES3MaterialPass.h"
#include "GLES3MaterialManager.h"

#include "GLES3DescriptorSet.h"
#include "GLES3DescriptorSetManager.h"
#include "GLES3DescriptorLayout.h"
#include "GLES3DescriptorLayoutManager.h"

#include "GLES3Semaphore.h"
#include "GLES3CommandBuffer.h"
#include "GLES3CommandBufferManager.h"


#define GLES3Renderer() ((CGLES3Renderer *)CGfxRenderer::GetInstance())


class CGLES3Renderer : public CGfxRenderer
{
public:
	CGLES3Renderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CGLES3Renderer(void);


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
	CGfxStorageBufferPtr NewStorageBuffer(size_t size);

	CGfxInstanceBufferPtr NewInstanceBuffer(uint32_t instanceFormat, int instanceBinding = 1);
	CGfxMultiInstanceBufferPtr NewMultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding = 1, int count = 3);

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding = 0);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding = 1);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom);
	CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0, int numLevels = INT_MAX);

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
	void CmdBindMesh(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshPtr ptrMesh);
	void CmdBindMeshDraw(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);

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

	void CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	void CmdUpdateInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size);

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
	CGLES3SwapChain* m_pSwapChain;

private:
	CGLES3ShaderManager* m_pShaderManager;
	CGLES3PipelineComputeManager* m_pPipelineComputeManager;
	CGLES3PipelineGraphicsManager* m_pPipelineGraphicsManager;
	CGLES3DescriptorLayoutManager* m_pDescriptorLayoutManager;
	CGLES3SamplerManager* m_pSamplerManager;
	CGLES3Texture2DManager* m_pTexture2DManager;
	CGLES3Texture2DArrayManager* m_pTexture2DArrayManager;
	CGLES3TextureCubemapManager* m_pTextureCubemapManager;
	CGLES3UniformBufferManager* m_pUniformBufferManager;
	CGLES3StorageBufferManager* m_pStorageBufferManager;
	CGLES3InstanceBufferManager* m_pInstanceBufferManager;
	CGLES3MeshManager* m_pMeshManager;
	CGLES3MeshDrawManager* m_pMeshDrawManager;
	CGLES3MaterialManager* m_pMaterialManager;
	CGLES3RenderPassManager* m_pRenderPassManager;
	CGLES3RenderTextureManager* m_pRenderTextureManager;
	CGLES3FrameBufferManager* m_pFrameBufferManager;
	CGLES3DescriptorSetManager* m_pDescriptorSetManager;
	CGLES3CommandBufferManager* m_pCommandBufferManager;
};
