#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


static const ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_COUNT] = {
	{ VERTEX_ATTRIBUTE_POSITION,  4, 3, 0, GFX_DATAFORMAT_VEC3_SFLOAT, "inPosition"  },
	{ VERTEX_ATTRIBUTE_NORMAL,    4, 3, 1, GFX_DATAFORMAT_VEC3_SFLOAT, "inNormal"    },
	{ VERTEX_ATTRIBUTE_BINORMAL,  4, 3, 2, GFX_DATAFORMAT_VEC3_SFLOAT, "inBinormal"  },
	{ VERTEX_ATTRIBUTE_COLOR,     4, 3, 3, GFX_DATAFORMAT_VEC3_SFLOAT, "inColor"     },
	{ VERTEX_ATTRIBUTE_TEXCOORD0, 4, 2, 4, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord0" },
	{ VERTEX_ATTRIBUTE_TEXCOORD1, 4, 2, 5, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord1" },
};

static const ATTRIBUTE instanceAttributes[INSTANCE_ATTRIBUTE_COUNT] = {
	// Transform
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 4, 8,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol0" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 4, 9,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol1" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 4, 10, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol2" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 4, 11, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol3" },

	// PointLight
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR,      4, 4, 12, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightColor"       },
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION,4, 4, 13, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightAttenuation" },
};


const uint32_t DEFAULT_PASS_NAME = HashValue("Default");
const uint32_t FORWARD_LIGHTING_PASS_NAME = HashValue("ForwardLighting");
const uint32_t SHADOW_PASS_NAME = HashValue("Shadow");


CRenderSolution::CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)

	, m_pMainCamera(nullptr)
	, m_pMainCameraQueue(nullptr)

	, m_pEngineUniform(nullptr)
	, m_pMainCameraUniform(nullptr)
	, m_pShadowCameraUniform{ nullptr }

	, m_bEnableMSAA(false)
	, m_solution(RENDER_SOLUTION_DEFAULT)

	, m_pPassDefault(nullptr)
	, m_pPassForwardLighting(nullptr)

	, m_pPassShadow(nullptr)
{
	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	CreateColorAttachments();
	CreateShadowAttachments(2048, 2048);

	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	m_pMainCamera = new CGfxCamera;
	m_pMainCameraQueue = new CGfxRenderQueue;

	m_pEngineUniform = new CGfxUniformEngine;
	m_pMainCameraUniform = new CGfxUniformCamera;
	m_pShadowCameraUniform[0] = new CGfxUniformCamera;
	m_pShadowCameraUniform[1] = new CGfxUniformCamera;
	m_pShadowCameraUniform[2] = new CGfxUniformCamera;
	m_pShadowCameraUniform[3] = new CGfxUniformCamera;

	m_pPassDefault = new CPassDefault(this);
	m_pPassForwardLighting = new CPassForwardLighting(this);
	m_pPassShadow = new CPassShadow(this);
}

CRenderSolution::~CRenderSolution(void)
{
	DestroyColorAttachments();
	DestroyShadowAttachments();

	delete m_pPassDefault;
	delete m_pPassForwardLighting;
	delete m_pPassShadow;

	delete m_pMainCamera;
	delete m_pMainCameraQueue;

	delete m_pEngineUniform;
	delete m_pMainCameraUniform;
	delete m_pShadowCameraUniform[0];
	delete m_pShadowCameraUniform[1];
	delete m_pShadowCameraUniform[2];
	delete m_pShadowCameraUniform[3];

	delete m_pRenderer;
}

void CRenderSolution::CreateColorAttachments(void)
{
	const int samples = 4;

	const int width = GfxRenderer()->GetSwapChain()->GetWidth();
	const int height = GfxRenderer()->GetSwapChain()->GetHeight();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrPresentTexture[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
		m_ptrDepthStencilTexture[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("DepthStencilTexture(%d)", indexFrame));
		m_ptrDepthStencilTexture[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, width, height);
		/*
		m_ptrColorTextureMSAA[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("ColorTextureMSAA(%d)", indexFrame));
		m_ptrColorTextureMSAA[indexFrame]->Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, width, height, samples);
		m_ptrDepthStencilTextureMSAA[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("DepthStencilTextureMSAA(%d)", indexFrame));
		m_ptrDepthStencilTextureMSAA[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, width, height, samples);
		*/
	}
}

void CRenderSolution::DestroyColorAttachments(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrPresentTexture[indexFrame].Release();
		m_ptrDepthStencilTexture[indexFrame].Release();
		/*
		m_ptrColorTextureMSAA[indexFrame].Release();
		m_ptrDepthStencilTextureMSAA[indexFrame].Release();
		*/
	}
}

void CRenderSolution::CreateShadowAttachments(int width, int height)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrShadowMapTexture[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("ShadowMap(%d)", indexFrame));
		m_ptrShadowMapTexture[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, width, height);
	}
}

void CRenderSolution::DestroyShadowAttachments(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrShadowMapTexture[indexFrame].Release();
	}
}

CGfxCamera* CRenderSolution::GetMainCamera(void) const
{
	return m_pMainCamera;
}

CGfxRenderQueue* CRenderSolution::GetMainCameraQueue(void) const
{
	return m_pMainCameraQueue;
}

CGfxUniformEngine* CRenderSolution::GetEngineUniform(void) const
{
	return m_pEngineUniform;
}

CGfxUniformCamera* CRenderSolution::GetMainCameraUniform(void) const
{
	return m_pMainCameraUniform;
}

CGfxUniformCamera* CRenderSolution::GetShadowCameraUniform(int indexLevel) const
{
	return indexLevel >= 0 && indexLevel < 4 ? m_pShadowCameraUniform[indexLevel] : nullptr;
}

CGfxRenderTexturePtr CRenderSolution::GetPresentTexture(int indexFrame) const
{
	return m_ptrPresentTexture[indexFrame % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
}

CGfxRenderTexturePtr CRenderSolution::GetDepthStencilTexture(int indexFrame) const
{
	return m_ptrDepthStencilTexture[indexFrame % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
}

CGfxRenderTexturePtr CRenderSolution::GetColorTextureMSAA(int indexFrame) const
{
	return m_ptrColorTextureMSAA[indexFrame % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
}

CGfxRenderTexturePtr CRenderSolution::GetDepthStencilTextureMSAA(int indexFrame) const
{
	return m_ptrDepthStencilTextureMSAA[indexFrame % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
}

CGfxRenderTexturePtr CRenderSolution::GetShadowMapTexture(int indexFrame) const
{
	return m_ptrShadowMapTexture[indexFrame % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
}

void CRenderSolution::SetTime(float t, float dt)
{
	m_pEngineUniform->SetTime(t, dt);
}

void CRenderSolution::SetCameraScissor(float x, float y, float width, float height)
{
	m_pMainCamera->SetScissor(x, y, width, height);
}

void CRenderSolution::SetCameraViewport(float x, float y, float width, float height)
{
	m_pMainCamera->SetViewport(x, y, width, height);
	m_pMainCameraUniform->SetScreen(width, height);
}

void CRenderSolution::SetCameraPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pMainCamera->SetPerspective(fovy, aspect, zNear, zFar);
	m_pMainCameraUniform->SetPerspective(fovy, aspect, zNear, zFar);
}

void CRenderSolution::SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pMainCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pMainCameraUniform->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolution::SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pMainCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pMainCameraUniform->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolution::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pEngineUniform->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CRenderSolution::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pEngineUniform->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSolution::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pEngineUniform->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSolution::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetAmbientLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetMainPointLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainPointLightColor(red, green, blue);
}

void CRenderSolution::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pEngineUniform->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSolution::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pEngineUniform->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSolution::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainDirectLightColor(red, green, blue);
}

void CRenderSolution::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetMainFogColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainFogColor(red, green, blue);
}

void CRenderSolution::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pEngineUniform->SetMainFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSolution::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pEngineUniform->SetMainFogDistanceDensity(startDistance, endDistance, density);
}
