#include "VKRenderer.h"


CVKRenderer::CVKRenderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, pixelFormat)
	, m_pInstance(nullptr)
	, m_pDevice(nullptr)
	, m_pSwapChain(nullptr)

	, m_pShaderManager(nullptr)
	, m_pPipelineComputeManager(nullptr)
	, m_pPipelineGraphicsManager(nullptr)
	, m_pSamplerManager(nullptr)
	, m_pTexture2DManager(nullptr)
	, m_pTexture2DArrayManager(nullptr)
	, m_pTextureCubeMapManager(nullptr)
	, m_pUniformBufferManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pMeshDrawManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pRenderPassManager(nullptr)
	, m_pRenderTextureManager(nullptr)
	, m_pFrameBufferManager(nullptr)
	, m_pCommandBufferManager(nullptr)

	, m_pCurrentMaterialPass(nullptr)
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)
{
	m_pInstance = new CVKInstance(hInstance, hWnd);
	m_pDevice = new CVKDevice(m_pInstance);

	m_pShaderManager = new CVKShaderManager(m_pDevice);
	m_pPipelineComputeManager = new CVKPipelineComputeManager(m_pDevice);
	m_pPipelineGraphicsManager = new CVKPipelineGraphicsManager(m_pDevice);
	m_pSamplerManager = new CVKSamplerManager(m_pDevice);
	m_pTexture2DManager = new CVKTexture2DManager(m_pDevice);
	m_pTexture2DArrayManager = new CVKTexture2DArrayManager(m_pDevice);
	m_pTextureCubeMapManager = new CVKTextureCubeMapManager(m_pDevice);
	m_pUniformBufferManager = new CVKUniformBufferManager(m_pDevice);
	m_pMeshManager = new CVKMeshManager(m_pDevice);
	m_pMeshDrawManager = new CVKMeshDrawManager(m_pDevice);
	m_pMaterialManager = new CVKMaterialManager(m_pDevice);
	m_pRenderPassManager = new CVKRenderPassManager(m_pDevice);
	m_pRenderTextureManager = new CVKRenderTextureManager(m_pDevice);
	m_pFrameBufferManager = new CVKFrameBufferManager(m_pDevice);
	m_pCommandBufferManager = new CVKCommandBufferManager(m_pDevice);

	m_pSwapChain = new CVKSwapChain(m_pDevice, width, height, pixelFormat);
}

CVKRenderer::~CVKRenderer(void)
{
	delete m_pSwapChain;

	delete m_pCommandBufferManager;
	delete m_pFrameBufferManager;
	delete m_pRenderTextureManager;
	delete m_pRenderPassManager;
	delete m_pMaterialManager;
	delete m_pMeshDrawManager;
	delete m_pMeshManager;
	delete m_pUniformBufferManager;
	delete m_pSamplerManager;
	delete m_pTexture2DManager;
	delete m_pTexture2DArrayManager;
	delete m_pTextureCubeMapManager;
	delete m_pPipelineComputeManager;
	delete m_pPipelineGraphicsManager;
	delete m_pShaderManager;

	delete m_pDevice;
	delete m_pInstance;
}

uint32_t CVKRenderer::GetLastError(void) const
{
	return CVKInstance::GetLastError();
}

CGfxSwapChain* CVKRenderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

bool CVKRenderer::IsSupportExtension(const char *extension) const
{
	return false;
}

CGfxShader* CVKRenderer::CreateShader(const char *szFileName, shader_kind kind)
{
	return m_pShaderManager->Create(szFileName, kind);
}

CGfxPipelineCompute* CVKRenderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return m_pPipelineComputeManager->Create(pComputeShader);
}

CGfxPipelineGraphics* CVKRenderer::CreatePipelineGraphics(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
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

CGfxRenderTexturePtr CVKRenderer::GetRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Get(name);
}

CGfxRenderTexturePtr CVKRenderer::NewRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Create(name);
}

CGfxMeshPtr CVKRenderer::GetMesh(uint32_t name)
{
	return m_pMeshManager->Get(name);
}

CGfxMeshPtr CVKRenderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CVKRenderer::NewMesh(const char *szFileName, uint32_t vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CVKRenderer::GetMeshDraw(uint32_t name)
{
	return m_pMeshDrawManager->Get(name);
}

CGfxMeshDrawPtr CVKRenderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	return m_pMeshDrawManager->Create(name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
}

CGfxMaterialPtr CVKRenderer::GetMaterial(uint32_t name)
{
	return m_pMaterialManager->Get(name);
}

CGfxMaterialPtr CVKRenderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CVKRenderer::NewMaterial(const char *szFileName, uint32_t vertexBinding, uint32_t instanceBinding)
{
	return m_pMaterialManager->Create(szFileName, vertexBinding, instanceBinding);
}

CGfxTexture2DPtr CVKRenderer::GetTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Get(name);
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Create(name);
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(const char *szFileName)
{
	return m_pTexture2DManager->Create(szFileName);
}

CGfxTexture2DArrayPtr CVKRenderer::GetTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Get(name);
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Create(name);
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(const char *szFileName)
{
	return m_pTexture2DArrayManager->Create(szFileName);
}

CGfxTextureCubeMapPtr CVKRenderer::GetTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Get(name);
}

CGfxTextureCubeMapPtr CVKRenderer::NewTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Create(name);
}

CGfxTextureCubeMapPtr CVKRenderer::NewTextureCubeMap(const char *szFileName)
{
	return m_pTextureCubeMapManager->Create(szFileName);
}

CGfxUniformBufferPtr CVKRenderer::NewUniformBuffer(size_t size)
{
	return m_pUniformBufferManager->Create(size);
}

CGfxCommandBufferPtr CVKRenderer::NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->Create(pool, bMainCommandBuffer);
}

bool CVKRenderer::CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	return ptrCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

bool CVKRenderer::CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdNextSubpass();
}

bool CVKRenderer::CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdEndRenderPass();
}

bool CVKRenderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute)
{
	return ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

bool CVKRenderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics)
{
	return ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

bool CVKRenderer::CmdBindMaterialPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMaterialPtr ptrMaterial, uint32_t namePass)
{
	return ptrCommandBuffer->CmdBindMaterialPass(ptrMaterial, namePass);
}

bool CVKRenderer::CmdBindUniformBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
{
	return ptrCommandBuffer->CmdBindUniformBuffer(ptrUniformBuffer, nameUniform);
}

bool CVKRenderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0)
{
	return ptrCommandBuffer->CmdUniform1i(szName, v0);
}

bool CVKRenderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1)
{
	return ptrCommandBuffer->CmdUniform2i(szName, v0, v1);
}

bool CVKRenderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2)
{
	return ptrCommandBuffer->CmdUniform3i(szName, v0, v1, v2);
}

bool CVKRenderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3)
{
	return ptrCommandBuffer->CmdUniform4i(szName, v0, v1, v2, v3);
}

bool CVKRenderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0)
{
	return ptrCommandBuffer->CmdUniform1f(szName, v0);
}

bool CVKRenderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1)
{
	return ptrCommandBuffer->CmdUniform2f(szName, v0, v1);
}

bool CVKRenderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2)
{
	return ptrCommandBuffer->CmdUniform3f(szName, v0, v1, v2);
}

bool CVKRenderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3)
{
	return ptrCommandBuffer->CmdUniform4f(szName, v0, v1, v2, v3);
}

bool CVKRenderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform1iv(szName, count, value);
}

bool CVKRenderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform2iv(szName, count, value);
}

bool CVKRenderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform3iv(szName, count, value);
}

bool CVKRenderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform4iv(szName, count, value);
}

bool CVKRenderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform1fv(szName, count, value);
}

bool CVKRenderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform2fv(szName, count, value);
}

bool CVKRenderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform3fv(szName, count, value);
}

bool CVKRenderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform4fv(szName, count, value);
}

bool CVKRenderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix2fv(szName, count, value);
}

bool CVKRenderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix3fv(szName, count, value);
}

bool CVKRenderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix4fv(szName, count, value);
}

bool CVKRenderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	return ptrCommandBuffer->CmdClearDepth(depth);
}

bool CVKRenderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CVKRenderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CVKRenderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CVKRenderer::CmdSetInstanceBufferData(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	return ptrCommandBuffer->CmdSetInstanceBufferData(ptrMeshDraw, pInstanceBuffer, size);
}

bool CVKRenderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	if (ptrCommandBuffer->CmdBindMeshDraw(ptrMeshDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawInstance(ptrMeshDraw) == false) {
		return false;
	}

	return true;
}

bool CVKRenderer::CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	if (ptrCommandBuffer->CmdBindMeshDraw(ptrMeshDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawIndirect(ptrMeshDraw) == false) {
		return false;
	}

	return true;
}

bool CVKRenderer::CmdDrawScreen(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

bool CVKRenderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	return ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

bool CVKRenderer::CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPresent();
}

bool CVKRenderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char *szMessage)
{
	return ptrCommandBuffer->CmdPushDebugGroup(szMessage);
}

bool CVKRenderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPopDebugGroup();
}

void CVKRenderer::Submit(const eastl::vector<CGfxCommandBufferPtr> &ptrCommandBuffers)
{
	m_pDevice->GetQueue()->Submit(ptrCommandBuffers, m_pSwapChain->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, m_pSwapChain->GetRenderDoneSemaphore(), m_pSwapChain->GetRenderDoneFence());
}

void CVKRenderer::AcquireNextFrame(void)
{
	m_pSwapChain->AcquireNextFrame();
}

void CVKRenderer::Present(void)
{
	m_pSwapChain->Present();
}

void CVKRenderer::BindMeshDraw(VkCommandBuffer vkCommandBuffer, CGfxMeshDraw *pMeshDraw)
{
	((CVKMeshDraw *)pMeshDraw)->Bind(vkCommandBuffer);
}

void CVKRenderer::BindPipelineCompute(VkCommandBuffer vkCommandBuffer, CGfxPipelineCompute *pPipelineCompute)
{
	if (m_pCurrentPipelineCompute != pPipelineCompute) {
		m_pCurrentPipelineCompute  = (CVKPipelineCompute *)pPipelineCompute;

		if (m_pCurrentPipelineCompute) {
			m_pCurrentPipelineCompute->Bind(vkCommandBuffer);
		}
	}
}

void CVKRenderer::BindPipelineGraphics(VkCommandBuffer vkCommandBuffer, CGfxPipelineGraphics *pPipelineGraphics)
{
	if (m_pCurrentPipelineGraphics != pPipelineGraphics) {
		m_pCurrentPipelineGraphics  = (CVKPipelineGraphics *)pPipelineGraphics;

		if (m_pCurrentPipelineGraphics) {
			m_pCurrentPipelineGraphics->Bind(vkCommandBuffer);
		}
	}
}

void CVKRenderer::BindMaterialPass(VkCommandBuffer vkCommandBuffer, CGfxMaterialPass *pPass)
{
	if (m_pCurrentMaterialPass != pPass) {
		m_pCurrentMaterialPass  = (CVKMaterialPass *)pPass;

		if (m_pCurrentMaterialPass && m_pCurrentPipelineGraphics) {
			m_pCurrentMaterialPass->GetDescriptorSet()->Update();
			m_pCurrentMaterialPass->Bind(vkCommandBuffer, m_pCurrentPipelineGraphics);
		}
	}
}
