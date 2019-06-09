#pragma once
#include "GfxHeader.h"

#include "GLES3/GL31.h"
#include "GLES3Helper.h"
#include "GLES3Extension.h"
#include "GLES3Definition.h"

#include "GLES3SwapChain.h"
#include "GLES3RenderPass.h"
#include "GLES3RenderPassManager.h"
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
#include "GLES3InstanceBuffer.h"
#include "GLES3IndirectBuffer.h"
#include "GLES3VertexArrayObject.h"

#include "GLES3UniformBuffer.h"
#include "GLES3UniformBufferManager.h"

#include "GLES3Sampler.h"
#include "GLES3SamplerManager.h"
#include "GLES3Texture.h"
#include "GLES3Texture2D.h"
#include "GLES3Texture2DArray.h"
#include "GLES3TextureCubemap.h"
#include "GLES3TextureManager.h"
#include "GLES3RenderTexture.h"
#include "GLES3RenderTextureManager.h"

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

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding = 0);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding = 1);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding = 0, int instanceBinding = 1, int baseLevel = 0, int numLevels = INT_MAX);

	CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set);

	CGfxDescriptorSetPtr GetDescriptorSet(uint32_t name);
	CGfxDescriptorSetPtr NewDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass);

	CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer);

public:
	bool BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer);
	bool BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass);
	bool EndRecord(CGfxCommandBufferPtr ptrCommandBuffer);

	// Outside RenderPass
	bool CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout);

	// Inside RenderPass
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

	bool CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdUpdateInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size);

	bool CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer);

	// Other
	bool CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage);
	bool CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer);

	// Submit
	void Submit(const CGfxCommandBufferPtr& ptrCommandBuffer);

public:
	void AcquireNextFrame(void);
	void Present(void);


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
	CGLES3MeshManager* m_pMeshManager;
	CGLES3MeshDrawManager* m_pMeshDrawManager;
	CGLES3MaterialManager* m_pMaterialManager;
	CGLES3RenderPassManager* m_pRenderPassManager;
	CGLES3RenderTextureManager* m_pRenderTextureManager;
	CGLES3FrameBufferManager* m_pFrameBufferManager;
	CGLES3DescriptorSetManager* m_pDescriptorSetManager;
	CGLES3CommandBufferManager* m_pCommandBufferManager;
};
