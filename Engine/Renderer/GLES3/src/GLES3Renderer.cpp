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
	, m_pTextureCubeMapManager(nullptr)
	, m_pUniformBufferManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pMeshDrawManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pRenderPassManager(nullptr)
	, m_pRenderTextureManager(nullptr)
	, m_pFrameBufferManager(nullptr)
	, m_pDescriptorSetManager(nullptr)
	, m_pCommandBufferManager(nullptr)

	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)
{
	CGLES3Helper::SetupExtensions();

	m_pShaderManager = new CGLES3ShaderManager;
	m_pPipelineComputeManager = new CGLES3PipelineComputeManager;
	m_pPipelineGraphicsManager = new CGLES3PipelineGraphicsManager;
	m_pDescriptorLayoutManager = new CGLES3DescriptorLayoutManager;
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
	delete m_pUniformBufferManager;
	delete m_pTextureCubeMapManager;
	delete m_pTexture2DArrayManager;
	delete m_pTexture2DManager;
	delete m_pSamplerManager;
	delete m_pShaderManager;
	delete m_pPipelineComputeManager;
	delete m_pPipelineGraphicsManager;
	delete m_pDescriptorLayoutManager;
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

CGfxPipelineGraphics* CGLES3Renderer::CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
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

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(const char* szFileName, int baseLevel)
{
	return m_pTexture2DManager->Create(szFileName, baseLevel);
}

CGfxTexture2DArrayPtr CGLES3Renderer::GetTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Get(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(uint32_t name)
{
	return m_pTexture2DArrayManager->Create(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(const char* szFileName, int baseLevel)
{
	return m_pTexture2DArrayManager->Create(szFileName, baseLevel);
}

CGfxTextureCubeMapPtr CGLES3Renderer::GetTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Get(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(uint32_t name)
{
	return m_pTextureCubeMapManager->Create(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(const char* szFileName, int baseLevel)
{
	return m_pTextureCubeMapManager->Create(szFileName, baseLevel);
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

CGfxMeshPtr CGLES3Renderer::GetMesh(uint32_t name)
{
	return m_pMeshManager->Get(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(const char* szFileName, uint32_t vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CGLES3Renderer::GetMeshDraw(uint32_t name)
{
	return m_pMeshDrawManager->Get(name);
}

CGfxMeshDrawPtr CGLES3Renderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	return m_pMeshDrawManager->Create(name, ptrMesh, nameDraw, instanceFormat, instanceBinding);
}

CGfxMaterialPtr CGLES3Renderer::GetMaterial(uint32_t name)
{
	return m_pMaterialManager->Get(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(const char* szFileName, uint32_t vertexBinding, uint32_t instanceBinding, int baseLevel)
{
	return m_pMaterialManager->Create(szFileName, vertexBinding, instanceBinding, baseLevel);
}

CGfxDescriptorLayoutPtr CGLES3Renderer::NewDescriptorLayout(uint32_t set)
{
	return m_pDescriptorLayoutManager->Create(set);
}

CGfxDescriptorSetPtr CGLES3Renderer::NewDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	return m_pDescriptorSetManager->Create(ptrDescriptorLayout);
}

CGfxDescriptorSetPtr CGLES3Renderer::NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	return m_pDescriptorSetManager->Create(pPipelineGraphics, pFrameBuffer, pRenderPass, indexSubpass);
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

bool CGLES3Renderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
{
	return ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

bool CGLES3Renderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
{
	return ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

bool CGLES3Renderer::CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	return ptrCommandBuffer->CmdBindDescriptorSet(ptrDescriptorSet);
}

bool CGLES3Renderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0)
{
	return ptrCommandBuffer->CmdUniform1i(name, v0);
}

bool CGLES3Renderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1)
{
	return ptrCommandBuffer->CmdUniform2i(name, v0, v1);
}

bool CGLES3Renderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2)
{
	return ptrCommandBuffer->CmdUniform3i(name, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3)
{
	return ptrCommandBuffer->CmdUniform4i(name, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0)
{
	return ptrCommandBuffer->CmdUniform1f(name, v0);
}

bool CGLES3Renderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1)
{
	return ptrCommandBuffer->CmdUniform2f(name, v0, v1);
}

bool CGLES3Renderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2)
{
	return ptrCommandBuffer->CmdUniform3f(name, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3)
{
	return ptrCommandBuffer->CmdUniform4f(name, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return ptrCommandBuffer->CmdUniform1iv(name, count, value);
}

bool CGLES3Renderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return ptrCommandBuffer->CmdUniform2iv(name, count, value);
}

bool CGLES3Renderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return ptrCommandBuffer->CmdUniform3iv(name, count, value);
}

bool CGLES3Renderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return ptrCommandBuffer->CmdUniform4iv(name, count, value);
}

bool CGLES3Renderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniform1fv(name, count, value);
}

bool CGLES3Renderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniform2fv(name, count, value);
}

bool CGLES3Renderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniform3fv(name, count, value);
}

bool CGLES3Renderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniform4fv(name, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniformMatrix2fv(name, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniformMatrix3fv(name, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return ptrCommandBuffer->CmdUniformMatrix4fv(name, count, value);
}

bool CGLES3Renderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGLES3Renderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGLES3Renderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	return ptrCommandBuffer->CmdClearDepth(depth);
}

bool CGLES3Renderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGLES3Renderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	return ptrCommandBuffer->CmdDrawInstance(ptrMeshDraw, pInstanceBuffer, size);
}

bool CGLES3Renderer::CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	return ptrCommandBuffer->CmdDrawIndirect(ptrMeshDraw, pInstanceBuffer, size);
}

bool CGLES3Renderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	return ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

bool CGLES3Renderer::CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPresent();
}

bool CGLES3Renderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage)
{
	return ptrCommandBuffer->CmdPushDebugGroup(szMessage);
}

bool CGLES3Renderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPopDebugGroup();
}

void CGLES3Renderer::Submit(const eastl::vector<CGfxCommandBufferPtr>& ptrCommandBuffers)
{
	for (int indexCommandBuffer = 0; indexCommandBuffer < ptrCommandBuffers.size(); indexCommandBuffer++) {
		ptrCommandBuffers[indexCommandBuffer]->Execute();
	}
}

void CGLES3Renderer::AcquireNextFrame(void)
{
	m_pSwapChain->AcquireNextFrame();
}

void CGLES3Renderer::Present(void)
{
	m_pSwapChain->Present();
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CGLES3Renderer::BindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	if (m_pCurrentPipelineCompute != pPipelineCompute) {
		m_pCurrentPipelineCompute = (CGLES3PipelineCompute*)pPipelineCompute;

		if (m_pCurrentPipelineCompute) {
			m_pCurrentPipelineCompute->Bind();
		}
	}
}

void CGLES3Renderer::BindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	if (m_pCurrentPipelineGraphics != pPipelineGraphics) {
		m_pCurrentPipelineGraphics = (CGLES3PipelineGraphics*)pPipelineGraphics;

		if (m_pCurrentPipelineGraphics) {
			m_pCurrentPipelineGraphics->Bind();
		}
	}
}

void CGLES3Renderer::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	if (m_pCurrentPipelineCompute) {
		m_pCurrentPipelineCompute->BindDescriptorSet(ptrDescriptorSet);
	}

	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->BindDescriptorSet(ptrDescriptorSet);
	}
}

CGLES3PipelineCompute* CGLES3Renderer::GetCurrentPipelineCompute(void) const
{
	return m_pCurrentPipelineCompute;
}

CGLES3PipelineGraphics* CGLES3Renderer::GetCurrentPipelineGraphics(void) const
{
	return m_pCurrentPipelineGraphics;
}
