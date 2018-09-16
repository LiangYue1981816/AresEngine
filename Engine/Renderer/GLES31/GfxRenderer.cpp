#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxVertexAttribute.h"


#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif

#define GLAPIENTRY APIENTRY
#define APIENTRY WINAPI
#define GLEWAPI extern __declspec(dllexport)
GLEWAPI GLenum GLAPIENTRY glewInit(void);

#ifdef __cplusplus
}
#endif
#endif


#define UNIFORM_ENGINE_NAME "Engine"
#define UNIFORM_CAMERA_NAME "Camera"


CGfxRenderer::CGfxRenderer(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath)
	: m_hDC(hDC)

	, m_pScreenMesh(NULL)
	, m_pGlobalMaterial(NULL)
	, m_pCurrentMaterial(NULL)

	, m_pUniformEngine(NULL)

	, m_pFrameBufferManager(NULL)
	, m_pProgramManager(NULL)
	, m_pSamplerManager(NULL)
	, m_pTextureManager(NULL)
	, m_pMaterialManager(NULL)
	, m_pMeshManager(NULL)
{
#ifdef _WIN32
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
		1,                              // version number 
		PFD_DRAW_TO_WINDOW |            // support window 
		PFD_SUPPORT_OPENGL |            // support OpenGL 
		PFD_DOUBLEBUFFER,               // double buffered 
		PFD_TYPE_RGBA,                  // RGBA type 
		32,                             // 32-bit color depth 
		0, 0, 0, 0, 0, 0,               // color bits ignored 
		0,                              // no alpha buffer 
		0,                              // shift bit ignored 
		0,                              // no accumulation buffer 
		0, 0, 0, 0,                     // accum bits ignored 
		24,                             // 24-bit z-buffer 
		8,                              // 8-bit stencil buffer 
		0,                              // no auxiliary buffer 
		PFD_MAIN_PLANE,                 // main layer 
		0,                              // reserved 
		0, 0, 0                         // layer masks ignored 
	};

	pixelFormat = ChoosePixelFormat((HDC)m_hDC, &pixelFormatDescriptor);
	SetPixelFormat((HDC)m_hDC, pixelFormat, &pixelFormatDescriptor);

	HGLRC hRC = wglCreateContext((HDC)m_hDC);
	wglMakeCurrent((HDC)m_hDC, hRC);

	GLenum err = glewInit();
#endif

	strcpy(m_szShaderPath, szShaderPath);
	strcpy(m_szTexturePath, szTexturePath);
	strcpy(m_szMaterialPath, szMaterialPath);
	strcpy(m_szMeshPath, szMeshPath);

	m_pScreenMesh = new CGfxMesh(0);
	m_pGlobalMaterial = new CGfxMaterial(0);

	m_pUniformEngine = new CGfxUniformEngine;

	m_pCameraManager = new CGfxCameraManager;
	m_pFrameBufferManager = new CGfxFrameBufferManager;
	m_pProgramManager = new CGfxProgramManager;
	m_pSamplerManager = new CGfxSamplerManager;
	m_pTextureManager = new CGfxTextureManager;
	m_pMaterialManager = new CGfxMaterialManager;
	m_pMeshManager = new CGfxMeshManager;

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	const static Vertex vertices[] = {
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f,  -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f,   1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
	};

	const static GLuint indices[] = {
		0, 1, 2, 2, 3, 0
	};

	m_pScreenMesh->CreateIndexBuffer(sizeof(indices), indices, false, GL_UNSIGNED_INT);
	m_pScreenMesh->CreateVertexBuffer(sizeof(vertices), vertices, false, VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0);
}

CGfxRenderer::~CGfxRenderer(void)
{
	delete m_pScreenMesh;
	delete m_pGlobalMaterial;

	delete m_pUniformEngine;

	delete m_pCameraManager;
	delete m_pMeshManager;
	delete m_pMaterialManager;
	delete m_pTextureManager;
	delete m_pSamplerManager;
	delete m_pProgramManager;
	delete m_pFrameBufferManager;
}

const char* CGfxRenderer::GetShaderFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szShaderPath, szFileName);
	return szFullPath;
}

const char* CGfxRenderer::GetTextureFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szTexturePath, szFileName);
	return szFullPath;
}

const char* CGfxRenderer::GetMaterialFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szMaterialPath, szFileName);
	return szFullPath;
}

const char* CGfxRenderer::GetMeshFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szMeshPath, szFileName);
	return szFullPath;
}

CGfxCamera* CGfxRenderer::GetCamera(const char *szName)
{
	return m_pCameraManager->GetCamera(szName);
}

CGfxProgram* CGfxRenderer::CreateProgram(const char *szVertexFileName, const char *szFragmentFileName)
{
	return m_pProgramManager->CreateProgram(szVertexFileName, szFragmentFileName);
}

CGfxSampler* CGfxRenderer::CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode)
{
	return m_pSamplerManager->CreateSampler(minFilter, magFilter, addressMode);
}

CGfxTexture2D* CGfxRenderer::CreateTexture2D(GLuint name)
{
	return m_pTextureManager->CreateTexture2D(name);
}

CGfxTexture2DArray* CGfxRenderer::CreateTexture2DArray(GLuint name)
{
	return m_pTextureManager->CreateTexture2DArray(name);
}

CGfxTextureCubeMap* CGfxRenderer::CreateTextureCubeMap(GLuint name)
{
	return m_pTextureManager->CreateTextureCubeMap(name);
}

CGfxTexture2D* CGfxRenderer::LoadTexture2D(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2D(szFileName);
}

CGfxTexture2DArray* CGfxRenderer::LoadTexture2DArray(const char *szFileName)
{
	return m_pTextureManager->LoadTexture2DArray(szFileName);
}

CGfxTextureCubeMap* CGfxRenderer::LoadTextureCubeMap(const char *szFileName)
{
	return m_pTextureManager->LoadTextureCubeMap(szFileName);
}

void CGfxRenderer::FreeTexture(CGfxTextureBase *pTexture)
{
	m_pTextureManager->FreeTexture(pTexture);
}

CGfxFrameBuffer* CGfxRenderer::CreateFrameBuffer(GLuint width, GLuint height)
{
	return m_pFrameBufferManager->CreateFrameBuffer(width, height);
}

void CGfxRenderer::FreeFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	m_pFrameBufferManager->DestroyFrameBuffer(pFrameBuffer);
}

CGfxMesh* CGfxRenderer::LoadMesh(const char *szFileName)
{
	return m_pMeshManager->LoadMesh(szFileName);
}

void CGfxRenderer::FreeMesh(CGfxMesh *pMesh)
{
	m_pMeshManager->FreeMesh(pMesh);
}

CGfxMaterial* CGfxRenderer::LoadMaterial(const char *szFileName)
{
	return m_pMaterialManager->LoadMaterial(szFileName);
}

void CGfxRenderer::FreeMaterial(CGfxMaterial *pMaterial)
{
	m_pMaterialManager->FreeMaterial(pMaterial);
}

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

bool CGfxRenderer::CmdBeginPass(CGfxCommandBuffer *pCommandBuffer, CGfxFrameBuffer *pFrameBuffer)
{
	return pCommandBuffer->CmdBeginPass(pFrameBuffer);
}

bool CGfxRenderer::CmdEndPass(CGfxCommandBuffer *pCommandBuffer)
{
	return pCommandBuffer->CmdEndPass();
}

bool CGfxRenderer::CmdSetScissor(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height)
{
	return pCommandBuffer->CmdSetScissor(x, y, width, height);
}

bool CGfxRenderer::CmdSetViewport(CGfxCommandBuffer *pCommandBuffer, int x, int y, int width, int height)
{
	return pCommandBuffer->CmdSetViewport(x, y, width, height);
}

bool CGfxRenderer::CmdSetCullFace(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum cullFace, GLenum frontFace)
{
	return pCommandBuffer->CmdSetCullFace(bEnable, cullFace, frontFace);
}

bool CGfxRenderer::CmdSetDepthTest(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum depthFunc)
{
	return pCommandBuffer->CmdSetDepthTest(bEnable, depthFunc);
}

bool CGfxRenderer::CmdSetDepthWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnable)
{
	return pCommandBuffer->CmdSetDepthWrite(bEnable);
}

bool CGfxRenderer::CmdSetColorWrite(CGfxCommandBuffer *pCommandBuffer, bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha)
{
	return pCommandBuffer->CmdSetColorWrite(bEnableRed, bEnableGreen, bEnableBlue, bEnableAlpha);
}

bool CGfxRenderer::CmdSetBlend(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLenum srcFactor, GLenum dstFactor)
{
	return pCommandBuffer->CmdSetBlend(bEnable, srcFactor, dstFactor);
}

bool CGfxRenderer::CmdSetPolygonOffset(CGfxCommandBuffer *pCommandBuffer, bool bEnable, GLfloat factor, GLfloat units)
{
	return pCommandBuffer->CmdSetPolygonOffset(bEnable, factor, units);
}

bool CGfxRenderer::CmdBindCamera(CGfxCommandBuffer *pCommandBuffer, CGfxCamera *pCamera)
{
	return pCommandBuffer->CmdBindCamera(pCamera);
}

bool CGfxRenderer::CmdBindMaterial(CGfxCommandBuffer *pCommandBuffer, CGfxMaterial *pMaterial)
{
	return pCommandBuffer->CmdBindMaterial(pMaterial);
}

bool CGfxRenderer::CmdBindInputTexture(CGfxCommandBuffer *pCommandBuffer, const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode)
{
	return pCommandBuffer->CmdBindInputTexture(szName, texture, minFilter, magFilter, addressMode);
}

bool CGfxRenderer::CmdClearDepth(CGfxCommandBuffer *pCommandBuffer, float depth)
{
	return pCommandBuffer->CmdClearDepth(depth);
}

bool CGfxRenderer::CmdClearColor(CGfxCommandBuffer *pCommandBuffer, float red, float green, float blue, float alpha)
{
	return pCommandBuffer->CmdClearColor(red, green, blue, alpha);
}

bool CGfxRenderer::CmdDrawInstance(CGfxCommandBuffer *pCommandBuffer, CGfxMesh *pMesh, int indexCount, int baseIndex)
{
	if (pCommandBuffer->CmdBindMesh(pMesh) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawInstance(GL_TRIANGLES, pMesh->GetIndexType(), indexCount, baseIndex, pMesh->GetInstanceCount()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawInstance(CGfxCommandBuffer *pCommandBuffer, CGfxMesh *pMesh, int indexCount, int baseIndex, eastl::vector<glm::mat4> &mtxTransforms)
{
	if (pCommandBuffer->CmdBindMesh(pMesh, mtxTransforms) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawInstance(GL_TRIANGLES, pMesh->GetIndexType(), indexCount, baseIndex, (GLsizei)mtxTransforms.size()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawIndirect(CGfxCommandBuffer *pCommandBuffer, CGfxMesh *pMesh, int indexCount, int baseIndex, GLsizei baseVertex)
{
	if (pCommandBuffer->CmdBindMesh(pMesh) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawIndirect(GL_TRIANGLES, pMesh->GetIndexType(), indexCount, baseIndex, baseVertex, pMesh->GetInstanceCount()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawIndirect(CGfxCommandBuffer *pCommandBuffer, CGfxMesh *pMesh, int indexCount, int baseIndex, GLsizei baseVertex, eastl::vector<glm::mat4> &mtxTransforms)
{
	if (pCommandBuffer->CmdBindMesh(pMesh, mtxTransforms) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawIndirect(GL_TRIANGLES, pMesh->GetIndexType(), indexCount, baseIndex, baseVertex, (GLsizei)mtxTransforms.size()) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdDrawScreen(CGfxCommandBuffer *pCommandBuffer)
{
	if (pCommandBuffer->CmdBindMesh(m_pScreenMesh) == false) {
		return false;
	}

	if (pCommandBuffer->CmdDrawElements(GL_TRIANGLES, m_pScreenMesh->GetIndexType(), m_pScreenMesh->GetIndexCount(), 0) == false) {
		return false;
	}

	return true;
}

bool CGfxRenderer::CmdExecute(CGfxCommandBuffer *pCommandBuffer, CGfxCommandBuffer *pSecondaryCommandBuffer)
{
	return pCommandBuffer->CmdExecute(pSecondaryCommandBuffer);
}

void CGfxRenderer::Update(void)
{
	m_pUniformEngine->Apply();
}

void CGfxRenderer::Submit(const CGfxCommandBuffer *pCommandBuffer)
{
	pCommandBuffer->Execute();
}

void CGfxRenderer::Present(void)
{
#ifdef _WIN32
	::SwapBuffers((HDC)m_hDC);
#endif
}

void CGfxRenderer::InvalidateFramebuffer(CGfxFrameBuffer *pFrameBuffer)
{
	if (pFrameBuffer) {
		pFrameBuffer->InvalidateFramebuffer();
	}
}

void CGfxRenderer::BindFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	if (pFrameBuffer) {
		pFrameBuffer->Bind();
	}
}

void CGfxRenderer::BindMesh(CGfxMesh *pMesh)
{
	if (pMesh) {
		pMesh->Bind();
	}
}

void CGfxRenderer::BindCamera(CGfxCamera *pCamera)
{
	if (m_pCurrentMaterial) {
		m_pCurrentMaterial->GetProgram()->BindUniformBuffer(HashValue(UNIFORM_CAMERA_NAME), pCamera->GetUniformCamera()->GetBuffer(), pCamera->GetUniformCamera()->GetSize());
	}
}

void CGfxRenderer::BindMaterial(CGfxMaterial *pMaterial)
{
	if (m_pCurrentMaterial != pMaterial) {
		m_pCurrentMaterial = pMaterial;

		if (m_pCurrentMaterial) {
			m_pCurrentMaterial->Bind();
			m_pCurrentMaterial->GetProgram()->BindUniformBuffer(HashValue(UNIFORM_ENGINE_NAME), m_pUniformEngine->GetBuffer(), m_pUniformEngine->GetSize());
		}
	}

	if (m_pCurrentMaterial) {
		m_pGlobalMaterial->BindUniforms(m_pCurrentMaterial->GetProgram());
		m_pGlobalMaterial->BindTextures(m_pCurrentMaterial->GetProgram(), m_pCurrentMaterial->GetTextureUnits());
	}
}

void CGfxRenderer::BindInputTexture(const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode)
{
	m_pGlobalMaterial->GetTexture2D(szName)->CreateExtern(texture);
	m_pGlobalMaterial->GetSampler(szName, minFilter, magFilter, addressMode);
}
