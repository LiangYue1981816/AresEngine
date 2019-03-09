#include "GLES3Renderer.h"


CGLES3Renderer::CGLES3Renderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, pixelFormat)

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
	CGLES3Helper::SetupExtensions();

	m_pShaderManager = new CGLES3ShaderManager;
	m_pPipelineComputeManager = new CGLES3PipelineComputeManager;
	m_pPipelineGraphicsManager = new CGLES3PipelineGraphicsManager;
	m_pSamplerManager = new CGLES3SamplerManager;
	m_pTexture2DManager = new CGLES3Texture2DManager;
	m_pTexture2DArrayManager = new CGLES3Texture2DArrayManager;
	m_pTextureCubeMapManager = new CGLES3TextureCubeMapManager;
	m_pUniformBufferManager = new CGLES3UniformBufferManager;
	m_pMeshManager = new CGLES3MeshManager;
	m_pMeshDrawManager = new CGLES3MeshDrawManager;
	m_pMaterialManager = new CGLES3MaterialManager;
	m_pRenderPassManager = new CGLES3RenderPassManager;
	m_pRenderTextureManager = new CGLES3RenderTextureManager;
	m_pFrameBufferManager = new CGLES3FrameBufferManager;
	m_pCommandBufferManager = new CGLES3CommandBufferManager;

	m_pSwapChain = new CGLES3SwapChain(hDC, width, height, pixelFormat);
}

CGLES3Renderer::~CGLES3Renderer(void)
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
}

uint32_t CGLES3Renderer::GetLastError(void) const
{
	return glGetError();
}

CGfxSwapChain* CGLES3Renderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

bool CGLES3Renderer::IsSupportExtension(const char *extension) const
{
	return CGLES3Helper::IsSupportExtension(extension);
}

CGfxShader* CGLES3Renderer::CreateShader(const char *szFileName, shader_kind kind)
{
	return m_pShaderManager->Create(szFileName, kind);
}

CGfxPipelineCompute* CGLES3Renderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return m_pPipelineComputeManager->Create(pComputeShader);
}

CGfxPipelineGraphics* CGLES3Renderer::CreatePipelineGraphics(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
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

CGfxRenderTexturePtr CGLES3Renderer::GetRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Get(name);
}

CGfxRenderTexturePtr CGLES3Renderer::NewRenderTexture(uint32_t name)
{
	return m_pRenderTextureManager->Create(name);
}

CGfxMeshPtr CGLES3Renderer::GetMesh(uint32_t name)
{
	return m_pMeshManager->Get(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(const char *szFileName, uint32_t vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CGLES3Renderer::GetMeshDraw(uint32_t name)
{
	return m_pMeshDrawManager->Get(name);
}

CGfxMeshDrawPtr CGLES3Renderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	return m_pMeshDrawManager->Create(name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
}

CGfxMaterialPtr CGLES3Renderer::GetMaterial(uint32_t name)
{
	return m_pMaterialManager->Get(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(const char *szFileName, uint32_t vertexBinding, uint32_t instanceBinding)
{
	return m_pMaterialManager->Create(szFileName, vertexBinding, instanceBinding);
}

CGfxTexture2DPtr CGLES3Renderer::GetTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Get(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(uint32_t name)
{
	return m_pTexture2DManager->Create(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(const char *szFileName)
{
	return m_pTexture2DManager->Create(szFileName);
}

CGfxTexture2DArrayPtr CGLES3Renderer::GetTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Get(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Create(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(const char *szFileName)
{
	return m_pTexture2DArrayManager->Create(szFileName);
}

CGfxTextureCubeMapPtr CGLES3Renderer::GetTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Get(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Create(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(const char *szFileName)
{
	return m_pTextureCubeMapManager->Create(szFileName);
}

CGfxUniformBufferPtr CGLES3Renderer::NewUniformBuffer(size_t size)
{
	return m_pUniformBufferManager->Create(size);
}

CGfxCommandBufferPtr CGLES3Renderer::NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->Create(bMainCommandBuffer);
}

bool CGLES3Renderer::CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	return ptrCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

bool CGLES3Renderer::CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdNextSubpass();
}

bool CGLES3Renderer::CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdEndRenderPass();
}

bool CGLES3Renderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute)
{
	return ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

bool CGLES3Renderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics)
{
	return ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

bool CGLES3Renderer::CmdBindMaterialPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMaterialPtr ptrMaterial, uint32_t namePass)
{
	return ptrCommandBuffer->CmdBindMaterialPass(ptrMaterial, namePass);
}

bool CGLES3Renderer::CmdBindUniformBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
{
	return ptrCommandBuffer->CmdBindUniformBuffer(ptrUniformBuffer, nameUniform);
}

bool CGLES3Renderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0)
{
	return ptrCommandBuffer->CmdUniform1i(szName, v0);
}

bool CGLES3Renderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1)
{
	return ptrCommandBuffer->CmdUniform2i(szName, v0, v1);
}

bool CGLES3Renderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2)
{
	return ptrCommandBuffer->CmdUniform3i(szName, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3)
{
	return ptrCommandBuffer->CmdUniform4i(szName, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0)
{
	return ptrCommandBuffer->CmdUniform1f(szName, v0);
}

bool CGLES3Renderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1)
{
	return ptrCommandBuffer->CmdUniform2f(szName, v0, v1);
}

bool CGLES3Renderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2)
{
	return ptrCommandBuffer->CmdUniform3f(szName, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3)
{
	return ptrCommandBuffer->CmdUniform4f(szName, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform1iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform2iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform3iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform4iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform1fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform2fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform3fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform4fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix2fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix3fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix4fv(szName, count, value);
}

bool CGLES3Renderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	return ptrCommandBuffer->CmdClearDepth(depth);
}

bool CGLES3Renderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGLES3Renderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGLES3Renderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGLES3Renderer::CmdSetInstanceBufferData(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	return ptrCommandBuffer->CmdSetInstanceBufferData(ptrMeshDraw, pInstanceBuffer, size);
}

bool CGLES3Renderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	if (ptrCommandBuffer->CmdBindMeshDraw(ptrMeshDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawInstance(ptrMeshDraw) == false) {
		return false;
	}

	return true;
}

bool CGLES3Renderer::CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw)
{
	if (ptrCommandBuffer->CmdBindMeshDraw(ptrMeshDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawIndirect(ptrMeshDraw) == false) {
		return false;
	}

	return true;
}

bool CGLES3Renderer::CmdDrawScreen(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

bool CGLES3Renderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	return ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

bool CGLES3Renderer::CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPresent();
}

bool CGLES3Renderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char *szMessage)
{
	return ptrCommandBuffer->CmdPushDebugGroup(szMessage);
}

bool CGLES3Renderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPopDebugGroup();
}

void CGLES3Renderer::Submit(const eastl::vector<CGfxCommandBufferPtr> &ptrCommandBuffers)
{
	for (int index = 0; index < ptrCommandBuffers.size(); index++) {
		ptrCommandBuffers[index]->Execute();
	}
}

void CGLES3Renderer::AcquireNextFrame(void)
{
	m_pSwapChain->AcquireNextFrame();
}

void CGLES3Renderer::Present(void)
{
	m_pSwapChain->Present();

	m_pCurrentMaterialPass = nullptr;
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CGLES3Renderer::BindMeshDraw(CGfxMeshDraw *pMeshDraw)
{
	((CGLES3MeshDraw *)pMeshDraw)->Bind();
}

void CGLES3Renderer::BindPipelineCompute(CGfxPipelineCompute *pPipelineCompute)
{
	if (m_pCurrentPipelineCompute != pPipelineCompute) {
		m_pCurrentPipelineCompute  = (CGLES3PipelineCompute *)pPipelineCompute;

		if (m_pCurrentPipelineCompute) {
			m_pCurrentPipelineCompute->Bind();
		}
	}
}

void CGLES3Renderer::BindPipelineGraphics(CGfxPipelineGraphics *pPipelineGraphics)
{
	if (m_pCurrentPipelineGraphics != pPipelineGraphics) {
		m_pCurrentPipelineGraphics  = (CGLES3PipelineGraphics *)pPipelineGraphics;

		if (m_pCurrentPipelineGraphics) {
			m_pCurrentPipelineGraphics->Bind();
		}
	}
}

void CGLES3Renderer::BindMaterialPass(CGfxMaterialPass *pMaterialPass)
{
	if (m_pCurrentMaterialPass != pMaterialPass) {
		m_pCurrentMaterialPass  = (CGLES3MaterialPass *)pMaterialPass;

		uint32_t indexTexUnit = 0;

		if (m_pCurrentMaterialPass && m_pCurrentPipelineGraphics) {
			m_pCurrentMaterialPass->Bind(m_pCurrentPipelineGraphics, indexTexUnit);
		}
	}
}

void CGLES3Renderer::BindUniformBuffer(CGfxUniformBuffer *pUniformBuffer, uint32_t nameUniform)
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->BindUniformBuffer(nameUniform, (CGLES3UniformBuffer *)pUniformBuffer, ((CGLES3UniformBuffer *)pUniformBuffer)->GetSize());
	}
}

void CGLES3Renderer::BindInputTexture(const char *szName, CGfxRenderTexture *pTexture)
{
	/*
	m_pGlobalMaterialPass->SetSampler(szName, GFX_FILTER_NEAREST, GFX_FILTER_LINEAR, GFX_SAMPLER_MIPMAP_MODE_NEAREST, GFX_SAMPLER_ADDRESS_MODE_REPEAT);
	m_pGlobalMaterialPass->SetTexture2D(szName, ((CGLES3RenderTexture *)pTexture)->GetTexture());
	*/
}

CGLES3PipelineCompute* CGLES3Renderer::GetCurrentPipelineCompute(void) const
{
	return m_pCurrentPipelineCompute;
}

CGLES3PipelineGraphics* CGLES3Renderer::GetCurrentPipelineGraphics(void) const
{
	return m_pCurrentPipelineGraphics;
}
