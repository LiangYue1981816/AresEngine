#include "GfxHeader.h"


CGLES3Renderer::CGLES3Renderer(void *hDC, int width, int height, uint32_t format)
	: CGfxRenderer(hDC, width, height, format)
	, m_pSwapChain(nullptr)

	, m_pMeshManager(nullptr)
	, m_pShaderManager(nullptr)
	, m_pSamplerManager(nullptr)
	, m_pTextureManager(nullptr)
	, m_pUniformManager(nullptr)
	, m_pPipelineManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pRenderPassManager(nullptr)
	, m_pFrameBufferManager(nullptr)
	, m_pCommandBufferManager(nullptr)

	, m_pGlobalMaterialPass(nullptr)
	, m_pCurrentMaterialPass(nullptr)
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)
{
	m_pMeshManager = new CGLES3MeshManager;
	m_pShaderManager = new CGLES3ShaderManager;
	m_pSamplerManager = new CGLES3SamplerManager;
	m_pTextureManager = new CGLES3TextureManager;
	m_pUniformManager = new CGLES3UniformManager;
	m_pPipelineManager = new CGLES3PipelineManager;
	m_pMaterialManager = new CGLES3MaterialManager;
	m_pRenderPassManager = new CGLES3RenderPassManager;
	m_pFrameBufferManager = new CGLES3FrameBufferManager;
	m_pCommandBufferManager = new CGLES3CommandBufferManager;

	m_pSwapChain = new CGLES3SwapChain(hDC, width, height, format);
	m_pGlobalMaterialPass = new CGLES3MaterialPass(INVALID_HASHVALUE);
}

CGLES3Renderer::~CGLES3Renderer(void)
{
	delete m_pSwapChain;

	delete m_pMeshManager;
	delete m_pShaderManager;
	delete m_pSamplerManager;
	delete m_pTextureManager;
	delete m_pUniformManager;
	delete m_pPipelineManager;
	delete m_pMaterialManager;
	delete m_pRenderPassManager;
	delete m_pFrameBufferManager;
	delete m_pCommandBufferManager;

	delete m_pGlobalMaterialPass;
}

CGfxSwapChain* CGLES3Renderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

CGLES3Shader* CGLES3Renderer::CreateShader(const char *szFileName, shader_kind kind)
{
	return m_pShaderManager->LoadShader(szFileName, kind);
}

CGLES3PipelineCompute* CGLES3Renderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return m_pPipelineManager->CreatePipelineCompute(pComputeShader);
}

CGLES3PipelineGraphics* CGLES3Renderer::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	return m_pPipelineManager->CreatePipelineGraphics(pVertexShader, pFragmentShader, state);
}

CGLES3Sampler* CGLES3Renderer::CreateSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	return m_pSamplerManager->CreateSampler(minFilter, magFilter, addressMode);
}

CGfxRenderPassPtr CGLES3Renderer::NewRenderPass(int numAttachments, int numSubpasses)
{
	return m_pRenderPassManager->CreateRenderPass(numAttachments, numSubpasses);
}

CGfxFrameBufferPtr CGLES3Renderer::NewFrameBuffer(int width, int height)
{
	return m_pFrameBufferManager->CreateFrameBuffer(width, height);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(const char *szFileName)
{
	return m_pMaterialManager->LoadMaterial(szFileName);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->CreateMesh(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(const char *szFileName, uint32_t instanceFormat)
{
	return m_pMeshManager->LoadMesh(szFileName, instanceFormat);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(uint32_t name)
{
	return m_pTextureManager->CreateTexture2D(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2D(szFileName);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(uint32_t name)
{
	return m_pTextureManager->CreateTexture2DArray(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2DArray(szFileName);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(uint32_t name)
{
	return m_pTextureManager->CreateTextureCubeMap(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(const char *szFileName)
{
	return m_pTextureManager->LoadTextureCubeMap(szFileName);
}

CGfxUniformEnginePtr CGLES3Renderer::NewUniformEngine(bool bDynamic)
{
	return m_pUniformManager->CreateUniformEngine(bDynamic);
}

CGfxUniformCameraPtr CGLES3Renderer::NewUniformCamera(bool bDynamic)
{
	return m_pUniformManager->CreateUniformCamera(bDynamic);
}

CGfxCommandBufferPtr CGLES3Renderer::NewCommandBuffer(bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->CreateCommandBuffer(bMainCommandBuffer);
}

bool CGLES3Renderer::CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
{
	return ptrCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

bool CGLES3Renderer::CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdNextSubpass();
}

bool CGLES3Renderer::CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdEndRenderPass();
}

bool CGLES3Renderer::CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute)
{
	return ptrCommandBuffer->CmdBindPipelineCompute(pPipelineCompute);
}

bool CGLES3Renderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics)
{
	return ptrCommandBuffer->CmdBindPipelineGraphics(pPipelineGraphics);
}

bool CGLES3Renderer::CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
{
	return ptrCommandBuffer->CmdBindMaterialPass(ptrMaterial, namePass);
}

bool CGLES3Renderer::CmdBindUniformEngine(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine)
{
	return ptrCommandBuffer->CmdBindUniformEngine(ptrUniformEngine);
}

bool CGLES3Renderer::CmdBindUniformCamera(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformCameraPtr &ptrUniformCamera)
{
	return ptrCommandBuffer->CmdBindUniformCamera(ptrUniformCamera);
}

bool CGLES3Renderer::CmdUniform1i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0)
{
	return ptrCommandBuffer->CmdUniform1i(szName, v0);
}

bool CGLES3Renderer::CmdUniform2i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1)
{
	return ptrCommandBuffer->CmdUniform2i(szName, v0, v1);
}

bool CGLES3Renderer::CmdUniform3i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2)
{
	return ptrCommandBuffer->CmdUniform3i(szName, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3)
{
	return ptrCommandBuffer->CmdUniform4i(szName, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0)
{
	return ptrCommandBuffer->CmdUniform1f(szName, v0);
}

bool CGLES3Renderer::CmdUniform2f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1)
{
	return ptrCommandBuffer->CmdUniform2f(szName, v0, v1);
}

bool CGLES3Renderer::CmdUniform3f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2)
{
	return ptrCommandBuffer->CmdUniform3f(szName, v0, v1, v2);
}

bool CGLES3Renderer::CmdUniform4f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3)
{
	return ptrCommandBuffer->CmdUniform4f(szName, v0, v1, v2, v3);
}

bool CGLES3Renderer::CmdUniform1iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform1iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform2iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform2iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform3iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform3iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform4iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return ptrCommandBuffer->CmdUniform4iv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform1fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform1fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform2fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform3fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniform4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniform4fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix2fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix3fv(szName, count, value);
}

bool CGLES3Renderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return ptrCommandBuffer->CmdUniformMatrix4fv(szName, count, value);
}

bool CGLES3Renderer::CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGLES3Renderer::CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGLES3Renderer::CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth)
{
	return ptrCommandBuffer->CmdClearDepth(depth);
}

bool CGLES3Renderer::CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGLES3Renderer::CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if (ptrCommandBuffer->CmdBindMesh(ptrMesh, indexDraw, pInstanceBuffer, size) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawInstance(GL_TRIANGLES, ptrMesh->GetIndexType(), ptrMesh->GetIndexOffset(indexDraw), ptrMesh->GetIndexCount(indexDraw), size / GetInstanceStride(ptrMesh->GetInstanceFormat())) == false) {
		return false;
	}

	return true;
}

bool CGLES3Renderer::CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if (ptrCommandBuffer->CmdBindMesh(ptrMesh, indexDraw, pInstanceBuffer, size) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawIndirect(GL_TRIANGLES, ptrMesh->GetIndexType(), ptrMesh->GetDrawCommandOffset(indexDraw)) == false) {
		return false;
	}

	return true;
}

bool CGLES3Renderer::CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return true;
}

bool CGLES3Renderer::CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer)
{
	return ptrCommandBuffer->CmdExecute(ptrSecondaryCommandBuffer);
}

void CGLES3Renderer::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer)
{
	ptrCommandBuffer->Execute();
}

void CGLES3Renderer::Present(void)
{
	m_pSwapChain->Present();

	m_pCurrentMaterialPass = nullptr;
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CGLES3Renderer::BindPipelineCompute(CGfxPipelineCompute *pPipelineCompute)
{
	if (m_pCurrentPipelineCompute != pPipelineCompute) {
		m_pCurrentPipelineCompute  = (CGLES3PipelineCompute *)pPipelineCompute;

		if (m_pCurrentPipelineCompute) {
			m_pCurrentPipelineCompute->Bind(nullptr);
		}
	}
}

void CGLES3Renderer::BindPipelineGraphics(CGfxPipelineGraphics *pPipelineGraphics)
{
	if (m_pCurrentPipelineGraphics != pPipelineGraphics) {
		m_pCurrentPipelineGraphics  = (CGLES3PipelineGraphics *)pPipelineGraphics;

		if (m_pCurrentPipelineGraphics) {
			m_pCurrentPipelineGraphics->Bind(nullptr);
		}
	}
}

void CGLES3Renderer::BindUniformEngine(CGfxUniformEngine *pUniformEngine)
{
	static const uint32_t uniformEngineName = HashValue(UNIFORM_ENGINE_NAME);

	if (pUniformEngine) {
		pUniformEngine->Apply();
		BindUniformBuffer(((CGLES3UniformEngine *)pUniformEngine)->GetUniformBuffer(), uniformEngineName);
	}
}

void CGLES3Renderer::BindUniformCamera(CGfxUniformCamera *pUniformCamera)
{
	static const uint32_t uniformCameraName = HashValue(UNIFORM_CAMERA_NAME);

	if (pUniformCamera) {
		pUniformCamera->Apply();
		BindUniformBuffer(((CGLES3UniformCamera *)pUniformCamera)->GetUniformBuffer(), uniformCameraName);
	}
}

void CGLES3Renderer::BindUniformBuffer(CGfxUniformBuffer *pUniformBuffer, uint32_t name)
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->BindUniformBuffer(name, (CGLES3UniformBuffer *)pUniformBuffer, ((CGLES3UniformBuffer *)pUniformBuffer)->GetSize());
	}
}

void CGLES3Renderer::BindMaterialPass(CGfxMaterialPass *pPass)
{
	if (m_pCurrentMaterialPass != pPass) {
		m_pCurrentMaterialPass  = (CGLES3MaterialPass *)pPass;

		if (m_pCurrentMaterialPass && m_pCurrentPipelineGraphics) {
			uint32_t indexTexUnit = 0;
			CGLES3MaterialPass::Bind(m_pCurrentPipelineGraphics, m_pCurrentMaterialPass, indexTexUnit);
			CGLES3MaterialPass::Bind(m_pCurrentPipelineGraphics, m_pGlobalMaterialPass, indexTexUnit);
		}
	}
}

void CGLES3Renderer::BindInputTexture(const char *szName, CGfxTextureBase *pTexture)
{
	m_pGlobalMaterialPass->SetTexture2D(szName, ((CGLES3TextureBase *)pTexture)->GetTexture());
	m_pGlobalMaterialPass->SetSampler(szName, GL_NEAREST, GL_LINEAR, GL_REPEAT);
}

void CGLES3Renderer::Uniform1i(uint32_t name, int v0) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform1i(name, v0);
	}
}

void CGLES3Renderer::Uniform2i(uint32_t name, int v0, int v1) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform2i(name, v0, v1);
	}
}

void CGLES3Renderer::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform3i(name, v0, v1, v2);
	}
}

void CGLES3Renderer::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform4i(name, v0, v1, v2, v3);
	}
}

void CGLES3Renderer::Uniform1f(uint32_t name, float v0) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform1f(name, v0);
	}
}

void CGLES3Renderer::Uniform2f(uint32_t name, float v0, float v1) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform2f(name, v0, v1);
	}
}

void CGLES3Renderer::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform3f(name, v0, v1, v2);
	}
}

void CGLES3Renderer::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform4f(name, v0, v1, v2, v3);
	}
}

void CGLES3Renderer::Uniform1iv(uint32_t name, int count, const int *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform1iv(name, count, value);
	}
}

void CGLES3Renderer::Uniform2iv(uint32_t name, int count, const int *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform2iv(name, count, value);
	}
}

void CGLES3Renderer::Uniform3iv(uint32_t name, int count, const int *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform3iv(name, count, value);
	}
}

void CGLES3Renderer::Uniform4iv(uint32_t name, int count, const int *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform4iv(name, count, value);
	}
}

void CGLES3Renderer::Uniform1fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform1fv(name, count, value);
	}
}

void CGLES3Renderer::Uniform2fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform2fv(name, count, value);
	}
}

void CGLES3Renderer::Uniform3fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform3fv(name, count, value);
	}
}

void CGLES3Renderer::Uniform4fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->Uniform4fv(name, count, value);
	}
}

void CGLES3Renderer::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->UniformMatrix2fv(name, count, value);
	}
}

void CGLES3Renderer::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->UniformMatrix3fv(name, count, value);
	}
}

void CGLES3Renderer::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->UniformMatrix4fv(name, count, value);
	}
}
