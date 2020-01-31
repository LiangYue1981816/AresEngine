#include "VKRenderer.h"


CVKRenderer::CVKRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, format)
	, m_pInstance(nullptr)
	, m_pDevice(nullptr)
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
	m_pInstance = new CVKInstance(hInstance, hWnd);
	m_pDevice = new CVKDevice(m_pInstance);

	m_pShaderManager = new CVKShaderManager(m_pDevice);
	m_pPipelineComputeManager = new CVKPipelineComputeManager(m_pDevice);
	m_pPipelineGraphicsManager = new CVKPipelineGraphicsManager(m_pDevice);
	m_pDescriptorLayoutManager = new CVKDescriptorLayoutManager(m_pDevice);
	m_pSamplerManager = new CVKSamplerManager(m_pDevice);
	m_pTexture2DManager = new CVKTexture2DManager(m_pDevice);
	m_pTexture2DArrayManager = new CVKTexture2DArrayManager(m_pDevice);
	m_pTextureCubemapManager = new CVKTextureCubemapManager(m_pDevice);
	m_pUniformBufferManager = new CVKUniformBufferManager(m_pDevice);
	m_pStorageBufferManager = new CVKStorageBufferManager(m_pDevice);
	m_pInstanceBufferManager = new CVKInstanceBufferManager(m_pDevice);
	m_pMeshManager = new CVKMeshManager(m_pDevice);
	m_pMeshDrawManager = new CVKMeshDrawManager(m_pDevice);
	m_pMaterialManager = new CVKMaterialManager(m_pDevice);
	m_pRenderPassManager = new CVKRenderPassManager(m_pDevice);
	m_pRenderTextureManager = new CVKRenderTextureManager(m_pDevice);
	m_pFrameBufferManager = new CVKFrameBufferManager(m_pDevice);
	m_pDescriptorSetManager = new CVKDescriptorSetManager(m_pDevice);
	m_pCommandBufferManager = new CVKCommandBufferManager(m_pDevice);

	m_pSwapChain = new CVKSwapChain(m_pDevice, width, height, format);
}

CVKRenderer::~CVKRenderer(void)
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

	delete m_pDevice;
	delete m_pInstance;
}

GfxApi CVKRenderer::GetAPI(void) const
{
	return GFX_API_VULKAN;
}

const glm::mat4& CVKRenderer::GetBaseMatrix(void) const
{
	static const glm::mat4 matrix = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));
	return matrix;
}

uint32_t CVKRenderer::GetLastError(void) const
{
	return m_pInstance->GetLastError();
}

CGfxSwapChain* CVKRenderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

bool CVKRenderer::IsSupportExtension(const char* extension) const
{
	return false;
}

CGfxShader* CVKRenderer::CreateShader(const char* szFileName, shader_kind kind)
{
	return m_pShaderManager->Create(szFileName, kind);
}

CGfxPipelineCompute* CVKRenderer::CreatePipelineCompute(const CGfxShader* pComputeShader)
{
	return m_pPipelineComputeManager->Create(pComputeShader);
}

CGfxPipelineGraphics* CVKRenderer::CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	return m_pPipelineGraphicsManager->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

CGfxSampler* CVKRenderer::CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	return m_pSamplerManager->Create(minFilter, magFilter, mipmapMode, addressMode);
}

CGfxFrameBufferPtr CVKRenderer::NewFrameBuffer(int width, int height, int numAttachments)
{
	return m_pFrameBufferManager->Create(width, height, numAttachments);
}

CGfxRenderPassPtr CVKRenderer::GetRenderPass(uint32_t name)
{
	return m_pRenderPassManager->Get(name);
}

CGfxRenderPassPtr CVKRenderer::NewRenderPass(uint32_t name, int numAttachments, int numSubpasses)
{
	return m_pRenderPassManager->Create(name, numAttachments, numSubpasses);
}

CGfxTexture2DPtr CVKRenderer::GetTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Get(name);
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Create(name);
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTexture2DManager->Create(szFileName, baseLevel, numLevels);
}

CGfxTexture2DArrayPtr CVKRenderer::GetTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Get(name);
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Create(name);
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTexture2DArrayManager->Create(szFileName, baseLevel, numLevels);
}

CGfxTextureCubemapPtr CVKRenderer::GetTextureCubemap(uint32_t name)
{
	return m_pTextureCubemapManager->Get(name);
}

CGfxTextureCubemapPtr CVKRenderer::NewTextureCubemap(uint32_t name)
{
	return m_pTextureCubemapManager->Create(name);
}

CGfxTextureCubemapPtr CVKRenderer::NewTextureCubemap(const char* szFileName, int baseLevel, int numLevels)
{
	return m_pTextureCubemapManager->Create(szFileName, baseLevel, numLevels);
}

CGfxRenderTexturePtr CVKRenderer::GetRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Get(name);
}

CGfxRenderTexturePtr CVKRenderer::NewRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Create(name);
}

CGfxUniformBufferPtr CVKRenderer::NewUniformBuffer(size_t size)
{
	return m_pUniformBufferManager->Create(size);
}

CGfxStorageBufferPtr CVKRenderer::NewStorageBuffer(size_t size)
{
	return m_pStorageBufferManager->Create(size);
}

CGfxInstanceBufferPtr CVKRenderer::NewInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
{
	return m_pInstanceBufferManager->Create(instanceFormat, instanceBinding);
}

CGfxMultiInstanceBufferPtr CVKRenderer::NewMultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count)
{
	return m_pInstanceBufferManager->Create(instanceFormat, instanceBinding, count);
}

CGfxMeshPtr CVKRenderer::GetMesh(uint32_t name)
{
	return m_pMeshManager->Get(name);
}

CGfxMeshPtr CVKRenderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CVKRenderer::NewMesh(const char* szFileName, int vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CVKRenderer::GetMeshDraw(uint32_t name)
{
	return m_pMeshDrawManager->Get(name);
}

CGfxMeshDrawPtr CVKRenderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
{
	return m_pMeshDrawManager->Create(name, ptrMesh, nameDraw, instanceFormat, instanceBinding);
}

CGfxMaterialPtr CVKRenderer::GetMaterial(uint32_t name)
{
	return m_pMaterialManager->Get(name);
}

CGfxMaterialPtr CVKRenderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CVKRenderer::NewMaterial(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom)
{
	return m_pMaterialManager->Create(name, ptrMaterialCopyFrom);
}

CGfxMaterialPtr CVKRenderer::NewMaterial(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel, int numLevels)
{
	return m_pMaterialManager->Create(szFileName, vertexBinding, instanceBinding, baseLevel, numLevels);
}

CGfxDescriptorLayoutPtr CVKRenderer::NewDescriptorLayout(uint32_t set)
{
	return m_pDescriptorLayoutManager->Create(set);
}

CGfxDescriptorSetPtr CVKRenderer::GetDescriptorSet(uint32_t name)
{
	return m_pDescriptorSetManager->Get(name);
}

CGfxDescriptorSetPtr CVKRenderer::NewDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	return m_pDescriptorSetManager->Create(name, ptrDescriptorLayout);
}

CGfxDescriptorSetPtr CVKRenderer::NewDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
{
	return m_pDescriptorSetManager->Create(name, ptrDescriptorSetCopyFrom);
}

CGfxDescriptorSetPtr CVKRenderer::NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	return m_pDescriptorSetManager->Create(pPipelineGraphics, pFrameBuffer, pRenderPass, indexSubpass);
}

CGfxCommandBufferPtr CVKRenderer::NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->Create(pool, bMainCommandBuffer);
}

void CVKRenderer::BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->Clearup();
	ptrCommandBuffer->BeginRecord();
}

void CVKRenderer::BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	ptrCommandBuffer->Clearup();
	ptrCommandBuffer->BeginRecord(ptrFrameBuffer, ptrRenderPass, indexSubpass);
}

void CVKRenderer::EndRecord(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->EndRecord();
}

void CVKRenderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CVKRenderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CVKRenderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CVKRenderer::CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
{
	ptrCommandBuffer->CmdSetImageLayout(ptrTexture, imageLayout);
}

void CVKRenderer::CmdSetBufferBarrier(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
{
	ptrCommandBuffer->CmdSetBufferBarrier(ptrBuffer, srcAccessFlags, dstAccessFlags);
}

void CVKRenderer::CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	ptrCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

void CVKRenderer::CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdNextSubpass();
}

void CVKRenderer::CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdEndRenderPass();
}

void CVKRenderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
{
	ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

void CVKRenderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
{
	ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

void CVKRenderer::CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	ptrCommandBuffer->CmdBindDescriptorSet(ptrDescriptorSet);
}

void CVKRenderer::CmdBindMesh(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshPtr ptrMesh)
{
	ptrCommandBuffer->CmdBindMesh(ptrMesh);
}

void CVKRenderer::CmdBindMeshDraw(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	ptrCommandBuffer->CmdBindMeshDraw(ptrMeshDraw);
}

void CVKRenderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0)
{
	ptrCommandBuffer->CmdUniform1i(name, v0);
}

void CVKRenderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1)
{
	ptrCommandBuffer->CmdUniform2i(name, v0, v1);
}

void CVKRenderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2)
{
	ptrCommandBuffer->CmdUniform3i(name, v0, v1, v2);
}

void CVKRenderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3)
{
	ptrCommandBuffer->CmdUniform4i(name, v0, v1, v2, v3);
}

void CVKRenderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0)
{
	ptrCommandBuffer->CmdUniform1f(name, v0);
}

void CVKRenderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1)
{
	ptrCommandBuffer->CmdUniform2f(name, v0, v1);
}

void CVKRenderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2)
{
	ptrCommandBuffer->CmdUniform3f(name, v0, v1, v2);
}

void CVKRenderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3)
{
	ptrCommandBuffer->CmdUniform4f(name, v0, v1, v2, v3);
}

void CVKRenderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform1iv(name, count, value);
}

void CVKRenderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform2iv(name, count, value);
}

void CVKRenderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform3iv(name, count, value);
}

void CVKRenderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	ptrCommandBuffer->CmdUniform4iv(name, count, value);
}

void CVKRenderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform1fv(name, count, value);
}

void CVKRenderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform2fv(name, count, value);
}

void CVKRenderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform3fv(name, count, value);
}

void CVKRenderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniform4fv(name, count, value);
}

void CVKRenderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix2fv(name, count, value);
}

void CVKRenderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix3fv(name, count, value);
}

void CVKRenderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	ptrCommandBuffer->CmdUniformMatrix4fv(name, count, value);
}

void CVKRenderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

void CVKRenderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

void CVKRenderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	ptrCommandBuffer->CmdClearDepth(depth);
}

void CVKRenderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

void CVKRenderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	ptrCommandBuffer->CmdDrawInstance(ptrMeshDraw);
}

void CVKRenderer::CmdUpdateInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	ptrCommandBuffer->CmdUpdateInstanceBuffer(ptrMeshDraw, pInstanceBuffer, size);
}

void CVKRenderer::CmdDispatch(CGfxCommandBufferPtr ptrCommandBuffer, int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
{
	ptrCommandBuffer->CmdDispatch(numLocalWorkGroupX, numLocalWorkGroupY, numLocalWorkGroupZ);
}

void CVKRenderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage)
{
	ptrCommandBuffer->CmdPushDebugGroup(szMessage);
}

void CVKRenderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	ptrCommandBuffer->CmdPopDebugGroup();
}

void CVKRenderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

void CVKRenderer::Submit(const CGfxCommandBufferPtr& ptrCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	if (pWaitSemaphore) {
		m_pDevice->GetQueue()->Submit(ptrCommandBuffer, ((CVKSemaphore *)pWaitSemaphore)->GetSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, ((CVKSemaphore *)ptrCommandBuffer->GetSemaphore())->GetSemaphore());
	}
	else {
		m_pDevice->GetQueue()->Submit(ptrCommandBuffer, VK_NULL_HANDLE, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, ((CVKSemaphore *)ptrCommandBuffer->GetSemaphore())->GetSemaphore());
	}
}

void CVKRenderer::AcquireNextFrame(void)
{
	m_pSwapChain->AcquireNextFrame();
}

void CVKRenderer::Present(const CGfxSemaphore* pWaitSemaphore)
{
	m_pSwapChain->Present(pWaitSemaphore);
}
