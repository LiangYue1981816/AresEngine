#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxVertexAttribute.h"


#define UNIFORM_ENGINE_NAME "Engine"
#define UNIFORM_CAMERA_NAME "Camera"


CGfxRenderer* CGfxRenderer::pInstance = NULL;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

CGfxRenderer::CGfxRenderer(void *hDC, const char *szShaderCachePath, int width, int height, uint32_t format)
	: m_pSwapChain(NULL)

	, m_pScreenMesh(NULL)
	, m_pGlobalPass(NULL)
	, m_pCurrentPass(NULL)
	, m_pCurrentPipeline(NULL)

	, m_pUniformEngine(NULL)

	, m_pResourceManager(NULL)
	, m_pMeshManager(NULL)
	, m_pShaderManager(NULL)
	, m_pSamplerManager(NULL)
	, m_pTextureManager(NULL)
	, m_pPipelineManager(NULL)
	, m_pMaterialManager(NULL)
	, m_pRenderPassManager(NULL)
	, m_pFrameBufferManager(NULL)
	, m_pDrawIndirectBufferManager(NULL)

	, m_pShaderCompiler(NULL)
{
	pInstance = this;

	m_pScreenMesh = new CGfxMesh(0);
	m_pGlobalPass = new CGfxMaterialPass(0);

	m_pUniformEngine = new CGfxUniformEngine;

	m_pResourceManager = new CGfxResourceManager;
	m_pMeshManager = new CGfxMeshManager;
	m_pShaderManager = new CGfxShaderManager;
	m_pSamplerManager = new CGfxSamplerManager;
	m_pTextureManager = new CGfxTextureManager;
	m_pPipelineManager = new CGfxPipelineManager;
	m_pMaterialManager = new CGfxMaterialManager;
	m_pRenderPassManager = new CGfxRenderPassManager;
	m_pFrameBufferManager = new CGfxFrameBufferManager;
	m_pDrawIndirectBufferManager = new CGfxDrawIndirectBufferManager;

	m_pSwapChain = new CGfxSwapChain(hDC, width, height, format);
	m_pShaderCompiler = new CGfxShaderCompiler(szShaderCachePath);

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	const static Vertex vertices[] = {
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
	};

	const static uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};

	m_pScreenMesh->CreateIndexBuffer(sizeof(indices), indices, false, GL_UNSIGNED_INT);
	m_pScreenMesh->CreateVertexBuffer(sizeof(vertices), vertices, false, VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0);

	GLResetContext();
}

CGfxRenderer::~CGfxRenderer(void)
{
	delete m_pSwapChain;
	delete m_pShaderCompiler;

	delete m_pScreenMesh;
	delete m_pGlobalPass;

	delete m_pUniformEngine;

	delete m_pResourceManager;
	delete m_pMeshManager;
	delete m_pShaderManager;
	delete m_pSamplerManager;
	delete m_pTextureManager;
	delete m_pPipelineManager;
	delete m_pMaterialManager;
	delete m_pRenderPassManager;
	delete m_pFrameBufferManager;
	delete m_pDrawIndirectBufferManager;
}

#pragma region SwapChain
CGfxSwapChain* CGfxRenderer::GetSwapChain(void) const
{
	return m_pSwapChain;
}
#pragma endregion

#pragma region Shader Compiler
CGfxShaderCompiler* CGfxRenderer::GetShaderCompiler(void) const
{
	return m_pShaderCompiler;
}
#pragma endregion

#pragma region Resource Path
void CGfxRenderer::SetResourcePath(const char *szPathName, const char *szExtName)
{
	m_pResourceManager->SetResourcePath(szPathName, szExtName);
}

const char* CGfxRenderer::GetResourceFullName(const char *szFileName) const
{
	return m_pResourceManager->GetResourceFullName(szFileName);
}
#pragma endregion

#pragma region Internal Gfx Resources
CGfxShader* CGfxRenderer::LoadShader(const char *szFileName, shaderc_shader_kind kind)
{
	return m_pShaderManager->LoadShader(szFileName, kind);
}

CGfxPipelineCompute* CGfxRenderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return m_pPipelineManager->CreatePipelineCompute(pComputeShader);
}

CGfxPipelineGraphics* CGfxRenderer::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state)
{
	return m_pPipelineManager->CreatePipelineGraphics(pVertexShader, pFragmentShader, state);
}

CGfxSampler* CGfxRenderer::CreateSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	return m_pSamplerManager->CreateSampler(minFilter, magFilter, addressMode);
}
#pragma endregion

#pragma region External Gfx Resources
CGfxTexture2DPtr CGfxRenderer::CreateTexture2D(uint32_t name)
{
	return m_pTextureManager->CreateTexture2D(name);
}

CGfxTexture2DArrayPtr CGfxRenderer::CreateTexture2DArray(uint32_t name)
{
	return m_pTextureManager->CreateTexture2DArray(name);
}

CGfxTextureCubeMapPtr CGfxRenderer::CreateTextureCubeMap(uint32_t name)
{
	return m_pTextureManager->CreateTextureCubeMap(name);
}

CGfxRenderPassPtr CGfxRenderer::CreateRenderPass(int numAttachments, int numSubpasses)
{
	return m_pRenderPassManager->CreateRenderPass(numAttachments, numSubpasses);
}

CGfxFrameBufferPtr CGfxRenderer::CreateFrameBuffer(int width, int height)
{
	return m_pFrameBufferManager->CreateFrameBuffer(width, height);
}

CGfxDrawIndirectBufferPtr CGfxRenderer::CreateDrawIndirectBuffer(const CGfxMesh *pMesh, int baseVertex, uint32_t firstIndex, uint32_t indexCount)
{
	return m_pDrawIndirectBufferManager->CreateDrawIndirectBuffer(pMesh, baseVertex, firstIndex, indexCount);
}

CGfxMeshPtr CGfxRenderer::LoadMesh(const char *szFileName)
{
	return m_pMeshManager->LoadMesh(szFileName);
}

CGfxMaterialPtr CGfxRenderer::LoadMaterial(const char *szFileName)
{
	return m_pMaterialManager->LoadMaterial(szFileName);
}

CGfxTexture2DPtr CGfxRenderer::LoadTexture2D(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2D(szFileName);
}

CGfxTexture2DArrayPtr CGfxRenderer::LoadTexture2DArray(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2DArray(szFileName);
}

CGfxTextureCubeMapPtr CGfxRenderer::LoadTextureCubeMap(const char *szFileName)
{
	return m_pTextureManager->LoadTextureCubeMap(szFileName);
}

void CGfxRenderer::DestroyMesh(CGfxMesh *pMesh)
{
	m_pMeshManager->DestroyMesh(pMesh);
}

void CGfxRenderer::DestroyMaterial(CGfxMaterial *pMaterial)
{
	m_pMaterialManager->DestroyMaterial(pMaterial);
}

void CGfxRenderer::DestroyTexture(CGfxTextureBase *pTexture)
{
	m_pTextureManager->DestroyTexture(pTexture);
}

void CGfxRenderer::DestroyRenderPass(CGfxRenderPass *pRenderPass)
{
	m_pRenderPassManager->DestroyRenderPass(pRenderPass);
}

void CGfxRenderer::DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	m_pFrameBufferManager->DestroyFrameBuffer(pFrameBuffer);
}

void CGfxRenderer::DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer)
{
	m_pDrawIndirectBufferManager->DestroyDrawIndirectBuffer(pBuffer);
}
#pragma endregion

#pragma region Features
void CGfxRenderer::SetTime(float t, float dt)
{
	m_pUniformEngine->SetTime(t, dt);
}

void CGfxRenderer::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pUniformEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CGfxRenderer::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pUniformEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CGfxRenderer::SetShadowDistance(float distance)
{
	m_pUniformEngine->SetShadowDistance(distance);
}

void CGfxRenderer::SetShadowResolution(float resolution)
{
	m_pUniformEngine->SetShadowResolution(resolution);
}

void CGfxRenderer::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pUniformEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CGfxRenderer::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pUniformEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CGfxRenderer::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pUniformEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CGfxRenderer::SetPointLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetPointLightColor(red, green, blue);
}

void CGfxRenderer::SetPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pUniformEngine->SetPointLightPosition(posx, posy, posz, radius);
}

void CGfxRenderer::SetPointLightAttenuation(float linear, float square, float constant)
{
	m_pUniformEngine->SetPointLightAttenuation(linear, square, constant);
}

void CGfxRenderer::SetDirectLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetDirectLightColor(red, green, blue);
}

void CGfxRenderer::SetDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pUniformEngine->SetDirectLightDirection(dirx, diry, dirz);
}

void CGfxRenderer::SetFogColor(float red, float green, float blue)
{
	m_pUniformEngine->SetFogColor(red, green, blue);
}

void CGfxRenderer::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pUniformEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CGfxRenderer::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pUniformEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}
#pragma endregion

#pragma region Commands
bool CGfxRenderer::CmdBeginRenderPass(CGfxCommandBuffer *pCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
{
	return pCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass);
}

bool CGfxRenderer::CmdEndRenderPass(CGfxCommandBuffer *pCommandBuffer)
{
	return pCommandBuffer->CmdEndRenderPass();
}

bool CGfxRenderer::CmdSetScissor(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height)
{
	return pCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGfxRenderer::CmdSetViewport(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height)
{
	return pCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGfxRenderer::CmdBindCamera(CGfxCommandBuffer *pCommandBuffer, CGfxCamera *pCamera)
{
	return pCommandBuffer->CmdBindCamera(pCamera);
}

bool CGfxRenderer::CmdBindPipeline(CGfxCommandBuffer *pCommandBuffer, CGfxPipelineBase *pPipeline)
{
	return pCommandBuffer->CmdBindPipeline(pPipeline);
}

bool CGfxRenderer::CmdBindMaterialPass(CGfxCommandBuffer *pCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
{
	return pCommandBuffer->CmdBindMaterialPass(ptrMaterial, namePass);
}

bool CGfxRenderer::CmdBindInputTexture(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	return pCommandBuffer->CmdBindInputTexture(szName, texture, minFilter, magFilter, addressMode);
}

bool CGfxRenderer::CmdUniform1f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0)
{
	return pCommandBuffer->CmdUniform1f(szName, v0);
}

bool CGfxRenderer::CmdUniform2f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1)
{
	return pCommandBuffer->CmdUniform2f(szName, v0, v1);
}

bool CGfxRenderer::CmdUniform3f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1, float v2)
{
	return pCommandBuffer->CmdUniform3f(szName, v0, v1, v2);
}

bool CGfxRenderer::CmdUniform4f(CGfxCommandBuffer *pCommandBuffer, const char *szName, float v0, float v1, float v2, float v3)
{
	return pCommandBuffer->CmdUniform4f(szName, v0, v1, v2, v3);
}

bool CGfxRenderer::CmdUniform1fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniform1fv(szName, count, value);
}

bool CGfxRenderer::CmdUniform2fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniform2fv(szName, count, value);
}

bool CGfxRenderer::CmdUniform3fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniform3fv(szName, count, value);
}

bool CGfxRenderer::CmdUniform4fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniform4fv(szName, count, value);
}

bool CGfxRenderer::CmdUniformMatrix2fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniformMatrix2fv(szName, count, value);
}

bool CGfxRenderer::CmdUniformMatrix3fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniformMatrix3fv(szName, count, value);
}

bool CGfxRenderer::CmdUniformMatrix4fv(CGfxCommandBuffer *pCommandBuffer, const char *szName, uint32_t count, const float *value)
{
	return pCommandBuffer->CmdUniformMatrix4fv(szName, count, value);
}

bool CGfxRenderer::CmdClearDepth(CGfxCommandBuffer *pCommandBuffer, float depth)
{
	return pCommandBuffer->CmdClearDepth(depth);
}

bool CGfxRenderer::CmdClearColor(CGfxCommandBuffer *pCommandBuffer, float red, float green, float blue, float alpha)
{
	return pCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGfxRenderer::CmdDrawInstance(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, uint32_t offset, int indexCount, const eastl::vector<glm::mat4> &mtxTransforms)
{
	if (pCommandBuffer->CmdBindMesh(ptrMesh, mtxTransforms) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawInstance(GL_TRIANGLES, ptrMesh->GetIndexType(), offset, indexCount, (int)mtxTransforms.size()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawIndirect(CGfxCommandBuffer *pCommandBuffer, const CGfxMeshPtr &ptrMesh, uint32_t offset, const eastl::vector<glm::mat4> &mtxTransforms)
{
	if (pCommandBuffer->CmdBindMesh(ptrMesh, mtxTransforms) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawIndirect(GL_TRIANGLES, ptrMesh->GetIndexType(), offset) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawScreen(CGfxCommandBuffer *pCommandBuffer)
{
	eastl::vector<glm::mat4> mtxTransforms;
	mtxTransforms.emplace_back(glm::mat4());

	if (pCommandBuffer->CmdBindMesh(m_pScreenMesh, mtxTransforms) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawElements(GL_TRIANGLES, m_pScreenMesh->GetIndexType(), 0, m_pScreenMesh->GetIndexCount()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdExecute(CGfxCommandBuffer *pCommandBuffer, CGfxCommandBuffer *pSecondaryCommandBuffer)
{
	return pCommandBuffer->CmdExecute(pSecondaryCommandBuffer);
}
#pragma endregion

#pragma region Draw
void CGfxRenderer::Submit(const CGfxCommandBuffer *pCommandBuffer)
{
	pCommandBuffer->Execute();
}

void CGfxRenderer::Present(void)
{
	m_pSwapChain->Present();

	m_pCurrentPass = NULL;
	m_pCurrentPipeline = NULL;
}
#pragma endregion

#pragma region Bind
void CGfxRenderer::BindPipeline(CGfxPipelineBase *pPipeline)
{
	static const uint32_t uniformEngineName = HashValue(UNIFORM_ENGINE_NAME);

	if (m_pCurrentPipeline != pPipeline) {
		m_pCurrentPipeline = (CGfxPipelineBase *)pPipeline;

		if (m_pCurrentPipeline) {
			m_pCurrentPipeline->BindPipeline();
			m_pCurrentPipeline->BindUniformBuffer(uniformEngineName, m_pUniformEngine->GetBuffer(), m_pUniformEngine->GetSize());
			m_pUniformEngine->Apply();
		}
	}
}

void CGfxRenderer::BindCamera(CGfxCamera *pCamera)
{
	static const uint32_t uniformCameraName = HashValue(UNIFORM_CAMERA_NAME);

	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->BindUniformBuffer(uniformCameraName, pCamera->GetUniformCamera()->GetBuffer(), pCamera->GetUniformCamera()->GetSize());
		pCamera->Apply();
	}
}

void CGfxRenderer::BindMaterialPass(CGfxMaterialPass *pPass)
{
	if (m_pCurrentPass != pPass) {
		m_pCurrentPass = (CGfxMaterialPass *)pPass;

		if (m_pCurrentPass && m_pCurrentPipeline) {
			uint32_t indexTexUnit = 0;
			CGfxMaterialPass::Bind(m_pCurrentPipeline, m_pCurrentPass, indexTexUnit);
			CGfxMaterialPass::Bind(m_pCurrentPipeline, m_pGlobalPass, indexTexUnit);
		}
	}
}

void CGfxRenderer::BindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	m_pGlobalPass->SetTexture2D(szName, texture);
	m_pGlobalPass->SetSampler(szName, minFilter, magFilter, addressMode);
}
#pragma endregion

#pragma region Uniform
void CGfxRenderer::Uniform1f(uint32_t name, float v0) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform1f(name, v0);
	}
}

void CGfxRenderer::Uniform2f(uint32_t name, float v0, float v1) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform2f(name, v0, v1);
	}
}

void CGfxRenderer::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform3f(name, v0, v1, v2);
	}
}

void CGfxRenderer::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform4f(name, v0, v1, v2, v3);
	}
}

void CGfxRenderer::Uniform1fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform1fv(name, count, value);
	}
}

void CGfxRenderer::Uniform2fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform2fv(name, count, value);
	}
}

void CGfxRenderer::Uniform3fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform3fv(name, count, value);
	}
}

void CGfxRenderer::Uniform4fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->Uniform4fv(name, count, value);
	}
}

void CGfxRenderer::UniformMatrix2fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->UniformMatrix2fv(name, count, value);
	}
}

void CGfxRenderer::UniformMatrix3fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->UniformMatrix3fv(name, count, value);
	}
}

void CGfxRenderer::UniformMatrix4fv(uint32_t name, uint32_t count, const float *value) const
{
	if (m_pCurrentPipeline) {
		m_pCurrentPipeline->UniformMatrix4fv(name, count, value);
	}
}
#pragma endregion
