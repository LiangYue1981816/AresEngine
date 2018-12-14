#include "GLES3Renderer.h"


CGLES3Renderer::CGLES3Renderer(void *hDC, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxRenderer(hDC, width, height, pixelFormat)

	, m_pSwapChain(nullptr)

	, m_pMeshManager(nullptr)
	, m_pMeshDrawManager(nullptr)
	, m_pShaderManager(nullptr)
	, m_pSamplerManager(nullptr)
	, m_pTextureManager(nullptr)
	, m_pPipelineManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pRenderPassManager(nullptr)
	, m_pFrameBufferManager(nullptr)
	, m_pUniformBufferManager(nullptr)
	, m_pCommandBufferManager(nullptr)

	, m_pGlobalMaterialPass(nullptr)
	, m_pCurrentMaterialPass(nullptr)
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)
{
	m_pMeshManager = new CGLES3MeshManager;
	m_pMeshDrawManager = new CGLES3MeshDrawManager;
	m_pShaderManager = new CGLES3ShaderManager;
	m_pSamplerManager = new CGLES3SamplerManager;
	m_pTextureManager = new CGLES3TextureManager;
	m_pPipelineManager = new CGLES3PipelineManager;
	m_pMaterialManager = new CGLES3MaterialManager;
	m_pRenderPassManager = new CGLES3RenderPassManager;
	m_pFrameBufferManager = new CGLES3FrameBufferManager;
	m_pUniformBufferManager = new CGLES3UniformBufferManager;
	m_pCommandBufferManager = new CGLES3CommandBufferManager;

	m_pSwapChain = new CGLES3SwapChain(hDC, width, height, pixelFormat);
	m_pGlobalMaterialPass = new CGLES3MaterialPass(INVALID_HASHVALUE);
}

CGLES3Renderer::~CGLES3Renderer(void)
{
	delete m_pSwapChain;
	delete m_pGlobalMaterialPass;

	delete m_pCommandBufferManager;
	delete m_pFrameBufferManager;
	delete m_pRenderPassManager;
	delete m_pMaterialManager;
	delete m_pSamplerManager;
	delete m_pTextureManager;
	delete m_pPipelineManager;
	delete m_pShaderManager;
	delete m_pMeshDrawManager;
	delete m_pMeshManager;
	delete m_pUniformBufferManager;
}

uint32_t CGLES3Renderer::GetLastError(void) const
{
	return glGetError();
}

CGfxSwapChain* CGLES3Renderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}

CGfxMaterialPass* CGLES3Renderer::GetGlobalPass(void) const
{
	return m_pGlobalMaterialPass;
}

CGfxShader* CGLES3Renderer::CreateShader(const char *szFileName, shader_kind kind)
{
	return m_pShaderManager->Create(szFileName, kind);
}

CGfxPipelineCompute* CGLES3Renderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return m_pPipelineManager->CreatePipelineCompute(pComputeShader);
}

CGfxPipelineGraphics* CGLES3Renderer::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	return m_pPipelineManager->CreatePipelineGraphics(pVertexShader, pFragmentShader, state);
}

CGfxSampler* CGLES3Renderer::CreateSampler(GfxMinFilter minFilter, GfxMagFilter magFilter, GfxAddressMode addressMode)
{
	return m_pSamplerManager->Create(minFilter, magFilter, addressMode);
}

CGfxRenderPassPtr CGLES3Renderer::NewRenderPass(int numAttachments, int numSubpasses)
{
	return m_pRenderPassManager->Create(numAttachments, numSubpasses);
}

CGfxFrameBufferPtr CGLES3Renderer::NewFrameBuffer(int width, int height)
{
	return m_pFrameBufferManager->Create(width, height);
}

bool CGLES3Renderer::IsHaveMesh(uint32_t name)
{
	return m_pMeshManager->IsHave(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(uint32_t name)
{
	return m_pMeshManager->Create(name);
}

CGfxMeshPtr CGLES3Renderer::NewMesh(const char *szFileName, uint32_t vertexBinding)
{
	return m_pMeshManager->Create(szFileName, vertexBinding);
}

CGfxMeshDrawPtr CGLES3Renderer::NewMeshDraw(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	return m_pMeshDrawManager->Create(ptrMesh, indexDraw, instanceFormat, instanceBinding);
}

bool CGLES3Renderer::IsHaveMaterial(uint32_t name)
{
	return m_pMaterialManager->IsHave(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(uint32_t name)
{
	return m_pMaterialManager->Create(name);
}

CGfxMaterialPtr CGLES3Renderer::NewMaterial(const char *szFileName)
{
	return m_pMaterialManager->Create(szFileName);
}

bool CGLES3Renderer::IsHaveTexture2D(uint32_t name)
{
	return m_pTextureManager->IsHaveTexture2D(name);
}

bool CGLES3Renderer::IsHaveTexture2DArray(uint32_t name)
{
	return m_pTextureManager->IsHaveTexture2DArray(name);
}

bool CGLES3Renderer::IsHaveTextureCubeMap(uint32_t name)
{
	return m_pTextureManager->IsHaveTextureCubeMap(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(uint32_t name)
{
	return m_pTextureManager->CreateTexture2D(name);
}

CGfxTexture2DPtr CGLES3Renderer::NewTexture2D(const char *szFileName)
{
	return m_pTextureManager->CreateTexture2D(szFileName);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(uint32_t name)
{
	return m_pTextureManager->CreateTexture2DArray(name);
}

CGfxTexture2DArrayPtr CGLES3Renderer::NewTexture2DArray(const char *szFileName)
{
	return m_pTextureManager->CreateTexture2DArray(szFileName);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(uint32_t name)
{
	return m_pTextureManager->CreateTextureCubeMap(name);
}

CGfxTextureCubeMapPtr CGLES3Renderer::NewTextureCubeMap(const char *szFileName)
{
	return m_pTextureManager->CreateTextureCubeMap(szFileName);
}

CGfxUniformBufferPtr CGLES3Renderer::NewUniformBuffer(size_t size, bool bDynamic)
{
	return m_pUniformBufferManager->Create(size, bDynamic);
}

CGfxCommandBufferPtr CGLES3Renderer::NewCommandBuffer(bool bMainCommandBuffer)
{
	return m_pCommandBufferManager->Create(bMainCommandBuffer);
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

bool CGLES3Renderer::CmdBindUniformBuffer(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform)
{
	return ptrCommandBuffer->CmdBindUniformBuffer(ptrUniformBuffer, nameUniform);
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

bool CGLES3Renderer::CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth)
{
	return ptrCommandBuffer->CmdClearDepth(depth);
}

bool CGLES3Renderer::CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return ptrCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGLES3Renderer::CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGLES3Renderer::CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return ptrCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGLES3Renderer::CmdSetInstanceBufferData(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	return ptrCommandBuffer->CmdSetInstanceBufferData(ptrMesh, indexDraw, pInstanceBuffer, size);
}

bool CGLES3Renderer::CmdSetDrawIndirectBufferData(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw, int instanceCount)
{
	return ptrCommandBuffer->CmdSetDrawIndirectBufferData(ptrMesh, indexDraw, instanceCount);
}

bool CGLES3Renderer::CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw, int instanceCount)
{
	if (ptrCommandBuffer->CmdBindMesh(ptrMesh, indexDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawInstance(GFX_DRAWMODE_TRIANGLES, ptrMesh->GetIndexType(), ptrMesh->GetIndexOffset(indexDraw), ptrMesh->GetIndexCount(indexDraw), instanceCount) == false) {
		return false;
	}

	return true;
}

bool CGLES3Renderer::CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const int indexDraw)
{
	if (ptrCommandBuffer->CmdBindMesh(ptrMesh, indexDraw) == false) {
		return false;
	}

	if (ptrCommandBuffer->CmdDrawIndirect(GFX_DRAWMODE_TRIANGLES, ptrMesh->GetIndexType(), ptrMesh->GetDrawCommandOffset(indexDraw)) == false) {
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

bool CGLES3Renderer::CmdPresent(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return ptrCommandBuffer->CmdPresent();
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

void CGLES3Renderer::BindUniformBuffer(CGfxUniformBuffer *pUniformBuffer, uint32_t nameUniform)
{
	if (m_pCurrentPipelineGraphics) {
		m_pCurrentPipelineGraphics->BindUniformBuffer(nameUniform, (CGLES3UniformBuffer *)pUniformBuffer, ((CGLES3UniformBuffer *)pUniformBuffer)->GetSize());
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
	m_pGlobalMaterialPass->SetSampler(szName, GFX_MINFILTER_NEAREST, GFX_MAGFILTER_LINEAR, GFX_ADDRESS_REPEAT);
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
