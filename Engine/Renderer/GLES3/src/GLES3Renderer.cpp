#include "GLES3Renderer.h"


CGLES3Renderer::CGLES3Renderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, format)
	, m_pSwapChain(nullptr)

	, m_pShaderManager(nullptr)
	, m_pPipelineComputeManager(nullptr)
	, m_pPipelineGraphicsManager(nullptr)
	, m_pDescriptorLayoutManager(nullptr)
	, m_pSamplerManager(nullptr)
	, m_pTexture2DManager(nullptr)
	, m_pTexture2DArrayManager(nullptr)
	, m_pTextureCubemapManager(nullptr)
	, m_pUniformBufferManager(nullptr)
	, m_pStorageBufferManager(nullptr)
	, m_pInstanceBufferManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pMeshDrawManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pRenderPassManager(nullptr)
	, m_pRenderTextureManager(nullptr)
	, m_pFrameBufferManager(nullptr)
	, m_pDescriptorSetManager(nullptr)
	, m_pCommandBufferManager(nullptr)
{
	CGLES3Helper::SetupExtensions();

	m_pShaderManager = new CGLES3ShaderManager;
	m_pPipelineComputeManager = new CGLES3PipelineComputeManager;
	m_pPipelineGraphicsManager = new CGLES3PipelineGraphicsManager;
	m_pDescriptorLayoutManager = new CGLES3DescriptorLayoutManager;
	m_pSamplerManager = new CGLES3SamplerManager;
	m_pTexture2DManager = new CGLES3Texture2DManager;
	m_pTexture2DArrayManager = new CGLES3Texture2DArrayManager;
	m_pTextureCubemapManager = new CGLES3TextureCubemapManager;
	m_pUniformBufferManager = new CGLES3UniformBufferManager;
	m_pStorageBufferManager = new CGLES3StorageBufferManager;
	m_pInstanceBufferManager = new CGLES3InstanceBufferManager;
	m_pMeshManager = new CGLES3MeshManager;
	m_pMeshDrawManager = new CGLES3MeshDrawManager;
	m_pMaterialManager = new CGLES3MaterialManager;
	m_pRenderPassManager = new CGLES3RenderPassManager;
	m_pRenderTextureManager = new CGLES3RenderTextureManager;
	m_pFrameBufferManager = new CGLES3FrameBufferManager;
	m_pDescriptorSetManager = new CGLES3DescriptorSetManager;
	m_pCommandBufferManager = new CGLES3CommandBufferManager;

	m_pSwapChain = new CGLES3SwapChain(hDC, width, height, format);
}

CGLES3Renderer::~CGLES3Renderer(void)
{
	delete m_pSwapChain;

	delete m_pCommandBufferManager;
	delete m_pDescriptorSetManager;
	delete m_pFrameBufferManager;
	delete m_pRenderTextureManager;
	delete m_pRenderPassManager;
	delete m_pMaterialManager;
	delete m_pMeshDrawManager;
	delete m_pMeshManager;
	delete m_pInstanceBufferManager;
	delete m_pStorageBufferManager;
	delete m_pUniformBufferManager;
	delete m_pTextureCubemapManager;
	delete m_pTexture2DArrayManager;
	delete m_pTexture2DManager;
	delete m_pSamplerManager;
	delete m_pShaderManager;
	delete m_pPipelineComputeManager;
	delete m_pPipelineGraphicsManager;
	delete m_pDescriptorLayoutManager;
}

GfxApi CGLES3Renderer::GetAPI(void) const
{
	return GFX_API_GLES3;
}

const glm::mat4& CGLES3Renderer::GetBaseMatrix(void) const
{
	static const glm::mat4 matrix;
	return matrix;
}

uint32_t CGLES3Renderer::GetLastError(void) const
{
	return glGetError();
}

CGfxSwapChain* CGLES3Renderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

bool CGLES3Renderer::IsSupportExtension(const char* extension) const
{
	return CGLES3Helper::IsSupportExtension(extension);
}

CGfxShader* CGLES3Renderer::CreateShader(const char* szFileName, shader_kind kind)
{
	return m_pShaderManager->Create(szFileName, kind);
}

CGfxPipelineCompute* CGLES3Renderer::CreatePipelineCompute(const CGfxShader* pComputeShader)
{
	return m_pPipelineComputeManager->Create(pComputeShader);
}

CGfxPipelineGraphics* CGLES3Renderer::CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	return m_pPipelineGraphicsManager->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

CGfxSampler* CGLES3Renderer::CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	return m_pSamplerManager->Create(minFilter, magFilter, mipmapMode, addressMode);
}

CGfxFrameBufferPtr CGLES3Renderer::NewFrameBuffer(int width, int height, int numAttachments)
{
	return m_pFrameBufferManager->Create(width, height, numAttachments);
}

CGfxRenderPassPtr CGLES3Renderer::GetRenderPass(uint32_t name)
{
	return m_pRenderPassManager->Get(name);
}

CGfxRenderPassPtr CGLES3Renderer::NewRenderPass(uint32_t name, int numAttachments, int numSubpasses)
{
	return m_pRenderPassManager->Create(name, numAttachments, numSubpasses);
}

CGfxTexture2DPtr CGLES3Renderer::GetTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Get(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Create(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTexture2DManager->Create(szFileName, baseLevel, numLevels);
}

CGfxTexture2DArrayPtr CGLES3Renderer::GetTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Get(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Create(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTexture2DArrayManager->Create(szFileName, baseLevel, numLevels);
}

CGfxTextureCubemapPtr CGLES3Renderer::GetTextureCubemap(uint32_t name)
{
	return m_pTextureCubemapManager->Get(name);
}

CGfxTextureCubemapPtr CGLES3Renderer::NewTextureCubemap(uint32_t name)
{
	return m_pTextureCubemapManager->Create(name);
}

CGfxTextureCubemapPtr CGLES3Renderer::NewTextureCubemap(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTextureCubemapManager->Create(szFileName, baseLevel, numLevels);
}

CGfxRenderTexturePtr CGLES3Renderer::GetRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Get(name);
}

CGfxRenderTexturePtr CGLES3Renderer::NewRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Create(name);
}

CGfxUniformBufferPtr CGLES3Renderer::NewUniformBuffer(size_t size)
{
	return m_pUniformBufferManager->Create(size);
}

CGfxStorageBufferPtr CGLES3Renderer::NewStorageBuffer(size_t size)
{
	return m_pStorageBufferManager->Create(size);
}

CGfxInstanceBufferPtr CGLES3Renderer::NewInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
{
	return m_pInstanceBufferManager->Create(instanceFormat, instanceBinding);
}

CGfxMultiInstanceBufferPtr CGLES3Renderer::NewMultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count)
{
	return m_pInstanceBufferManager->Create(instanceFormat, instanceBinding, count);
}

CGfxMeshPtr CGLES3Renderer::GetMesh(uint32_t name)
{
	return m_pMeshManager->Get(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(const char* szFileName, int vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CGLES3Renderer::GetMeshDraw(uint32_t name)
{
	return m_pMeshDrawManager->Get(name);
}

CGfxMeshDrawPtr CGLES3Renderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw)
{
	return m_pMeshDrawManager->Create(name, ptrMesh, nameDraw);
}

CGfxMaterialPtr CGLES3Renderer::GetMaterial(uint32_t name)
{
	return m_pMaterialManager->Get(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom)
{
	return m_pMaterialManager->Create(name, ptrMaterialCopyFrom);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel, int numLevels)
{
	return m_pMaterialManager->Create(szFileName, vertexBinding, instanceBinding, baseLevel, numLevels);
}

CGfxDescriptorLayoutPtr CGLES3Renderer::NewDescriptorLayout(uint32_t set)
{
	return m_pDescriptorLayoutManager->Create(set);
}

CGfxDescriptorSetPtr CGLES3Renderer::GetDescriptorSet(uint32_t name)
{
	return m_pDescriptorSetManager->Get(name);
}

CGfxDescriptorSetPtr CGLES3Renderer::NewDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	return m_pDescriptorSetManager->Create(name, ptrDescriptorLayout);
}

CGfxDescriptorSetPtr CGLES3Renderer::NewDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
{
	return m_pDescriptorSetManager->Create(name, ptrDescriptorSetCopyFrom);
}

CGfxDescriptorSetPtr CGLES3Renderer::NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	return m_pDescriptorSetManager->Create(pPipelineGraphics, pFrameBuffer, pRenderPass, indexSubpass);
}

CGfxCommandBufferPtr CGLES3Renderer::NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->Create(bMainCommandBuffer);
}

void CGLES3Renderer::BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->Clearup();
	ptrCommandBuffer->BeginRecord();
}

void CGLES3Renderer::BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	ptrCommandBuffer->Clearup();
	ptrCommandBuffer->BeginRecord(ptrFrameBuffer, ptrRenderPass, indexSubpass);
}

void CGLES3Renderer::EndRecord(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->EndRecord();
}

void CGLES3Renderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CGLES3Renderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CGLES3Renderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CGLES3Renderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CGLES3Renderer::CmdSetBufferBarrier(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
{
	ptrCommandBuffer->CmdSetBufferBarrier(ptrBuffer, srcAccessFlags, dstAccessFlags);
}

void CGLES3Renderer::CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	ptrCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

void CGLES3Renderer::CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdNextSubpass();
}

void CGLES3Renderer::CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdEndRenderPass();
}

void CGLES3Renderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
{
	ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

void CGLES3Renderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
{
	ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

void CGLES3Renderer::CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	ptrCommandBuffer->CmdBindDescriptorSet(ptrDescriptorSet);
}

void CGLES3Renderer::CmdBindIndexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer)
{
	ptrCommandBuffer->CmdBindIndexBuffer(ptrIndexBuffer);
}

void CGLES3Renderer::CmdBindVertexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer)
{
	ptrCommandBuffer->CmdBindVertexBuffer(ptrVertexBuffer);
}

void CGLES3Renderer::CmdBindInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset)
{
	ptrCommandBuffer->CmdBindInstanceBuffer(ptrInstanceBuffer, offset);
}

void CGLES3Renderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0)
{
	ptrCommandBuffer->CmdUniform1i(name, v0);
}

void CGLES3Renderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1)
{
	ptrCommandBuffer->CmdUniform2i(name, v0, v1);
}

void CGLES3Renderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2)
{
	ptrCommandBuffer->CmdUniform3i(name, v0, v1, v2);
}

void CGLES3Renderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3)
{
	ptrCommandBuffer->CmdUniform4i(name, v0, v1, v2, v3);
}

void CGLES3Renderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0)
{
	ptrCommandBuffer->CmdUniform1f(name, v0);
}

void CGLES3Renderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1)
{
	ptrCommandBuffer->CmdUniform2f(name, v0, v1);
}

void CGLES3Renderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2)
{
	ptrCommandBuffer->CmdUniform3f(name, v0, v1, v2);
}

void CGLES3Renderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3)
{
	ptrCommandBuffer->CmdUniform4f(name, v0, v1, v2, v3);
}

void CGLES3Renderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform1iv(name, count, value);
}

void CGLES3Renderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform2iv(name, count, value);
}

void CGLES3Renderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform3iv(name, count, value);
}

void CGLES3Renderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform4iv(name, count, value);
}

void CGLES3Renderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform1fv(name, count, value);
}

void CGLES3Renderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform2fv(name, count, value);
}

void CGLES3Renderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform3fv(name, count, value);
}

void CGLES3Renderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform4fv(name, count, value);
}

void CGLES3Renderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix2fv(name, count, value);
}

void CGLES3Renderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix3fv(name, count, value);
}

void CGLES3Renderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix4fv(name, count, value);
}

void CGLES3Renderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

void CGLES3Renderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

void CGLES3Renderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	ptrCommandBuffer->CmdClearDepth(depth);
}

void CGLES3Renderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

void CGLES3Renderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, int indexType, int indexOffset, int indexCount, int instanceCount)
{
	ptrCommandBuffer->CmdDrawInstance(indexType, indexOffset, indexCount, instanceCount);
}

void CGLES3Renderer::CmdDispatch(CGfxCommandBufferPtr ptrCommandBuffer, int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
{
	ptrCommandBuffer->CmdDispatch(numLocalWorkGroupX, numLocalWorkGroupY, numLocalWorkGroupZ);
}

void CGLES3Renderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage)
{
	ptrCommandBuffer->CmdPushDebugGroup(szMessage);
}

void CGLES3Renderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdPopDebugGroup();
}

void CGLES3Renderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

void CGLES3Renderer::Submit(const CGfxCommandBufferPtr& ptrCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	ptrCommandBuffer->Execute();
}

void CGLES3Renderer::AcquireNextFrame(void)
{
	m_pSwapChain->AcquireNextFrame();
}

void CGLES3Renderer::Present(const CGfxSemaphore* pWaitSemaphore)
{
	m_pSwapChain->Present(pWaitSemaphore);
}
