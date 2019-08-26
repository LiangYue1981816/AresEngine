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
const uint32_t SHADOWMAP_PASS_NAME = HashValue("ShadowMap");
const uint32_t SHADOWMAP_BLUR_PASS_NAME = HashValue("ShadowMapBlur");


CRenderSystem::CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)
	, m_pEngineUniform(nullptr)

	, m_pPassDefault(nullptr)
	, m_pPassForwardLighting(nullptr)
	, m_pPassShadowMap(nullptr)
	, m_pPassShadowMapBlur(nullptr)
{
	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		m_pEngineUniform = new CGfxUniformEngine;
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		m_pEngineUniform = new CGfxUniformEngine;
		break;
	}

	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	CreateRenderTexture();
	CreateRenderPass();
}

CRenderSystem::~CRenderSystem(void)
{
	DestroyRenderPass();
	DestroyRenderTexture();

	delete m_pEngineUniform;
	delete m_pRenderer;
}

CGfxUniformEngine* CRenderSystem::GetEngineUniform(void) const
{
	return m_pEngineUniform;
}

void CRenderSystem::CreateRenderTexture(void)
{
	m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR0] = GfxRenderer()->GetSwapChain()->GetFrameTexture(0);
	m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR1] = GfxRenderer()->GetSwapChain()->GetFrameTexture(1);
	m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR2] = GfxRenderer()->GetSwapChain()->GetFrameTexture(2);

	m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH] = GfxRenderer()->NewRenderTexture(HashValue("RENDER_TEXTURE_SWAPCHAIN_DEPTH"));
	m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());

	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_DEPTH] = GfxRenderer()->NewRenderTexture(HashValue("RENDER_TEXTURE_SHADOWMAP_DEPTH"));
	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR] = GfxRenderer()->NewRenderTexture(HashValue("RENDER_TEXTURE_SHADOWMAP_COLOR"));
	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR] = GfxRenderer()->NewRenderTexture(HashValue("RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR"));
	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_DEPTH]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 2048, 2048);
	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR]->Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, 2048, 2048);
	m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR]->Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, 2048, 2048);
}

void CRenderSystem::DestroyRenderTexture(void)
{
	for (int index = 0; index < RENDER_TEXTURE_COUNT; index++) {
		m_ptrRenderTextures[index].Release();
	}
}

CGfxRenderTexturePtr CRenderSystem::GetRenderTexture(RenderTextureType type)
{
	if ((int)type >= 0 && (int)type < RENDER_TEXTURE_COUNT) {
		return m_ptrRenderTextures[(int)type];
	}
	else {
		return nullptr;
	}
}

void CRenderSystem::CreateRenderPass(void)
{
	CPassDefault::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 1);
	CPassForwardLighting::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 1);
	CPassShadowMap::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassShadowMapBlur::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);

	m_pPassDefault = new CPassDefault(this);
	m_pPassDefault->SetFrameBuffer(0, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR0], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);
	m_pPassDefault->SetFrameBuffer(1, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR1], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);
	m_pPassDefault->SetFrameBuffer(2, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR2], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);

	m_pPassForwardLighting = new CPassForwardLighting(this);
	m_pPassForwardLighting->SetFrameBuffer(0, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR0], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);
	m_pPassForwardLighting->SetFrameBuffer(1, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR1], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);
	m_pPassForwardLighting->SetFrameBuffer(2, m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_COLOR2], m_ptrRenderTextures[RENDER_TEXTURE_SWAPCHAIN_DEPTH]);
//	m_pPassForwardLighting->SetInputShadowMapTexture(m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR]);
	m_pPassForwardLighting->SetInputShadowMapTexture(m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR]);

	m_pPassShadowMap = new CPassShadowMap(this);
	m_pPassShadowMap->SetFrameBuffer(m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR], m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_DEPTH]);

	m_pPassShadowMapBlur = new CPassShadowMapBlur(this);
	m_pPassShadowMapBlur->SetFrameBuffer(m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR]);
	m_pPassShadowMapBlur->SetInputShadowMapTexture(m_ptrRenderTextures[RENDER_TEXTURE_SHADOWMAP_COLOR]);
}

void CRenderSystem::DestroyRenderPass(void)
{
	CPassDefault::Destroy();
	CPassForwardLighting::Destroy();
	CPassShadowMap::Destroy();
	CPassShadowMapBlur::Destroy();

	delete m_pPassDefault;
	delete m_pPassForwardLighting;
	delete m_pPassShadowMap;
	delete m_pPassShadowMapBlur;
}

void CRenderSystem::SetTime(float t, float dt)
{
	m_pEngineUniform->SetTime(t, dt);
}

void CRenderSystem::SetEnvLightFactor(float factor)
{
	m_pEngineUniform->SetEnvLightFactor(factor);
}

void CRenderSystem::SetAmbientLightFactor(float factor)
{
	m_pEngineUniform->SetAmbientLightFactor(factor);
}

void CRenderSystem::SetMainPointLightFactor(float factor)
{
	m_pEngineUniform->SetMainPointLightFactor(factor);
}

void CRenderSystem::SetMainDirectLightFactor(float factor)
{
	m_pEngineUniform->SetMainDirectLightFactor(factor);
}

void CRenderSystem::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pEngineUniform->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSystem::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pEngineUniform->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSystem::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetAmbientLightDirection(dirx, diry, dirz);
}

void CRenderSystem::SetMainPointLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainPointLightColor(red, green, blue);
}

void CRenderSystem::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pEngineUniform->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSystem::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pEngineUniform->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSystem::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainDirectLightColor(red, green, blue);
}

void CRenderSystem::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSystem::SetMainFogColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainFogColor(red, green, blue);
}

void CRenderSystem::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pEngineUniform->SetMainFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSystem::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pEngineUniform->SetMainFogDistanceDensity(startDistance, endDistance, density);
}

void CRenderSystem::SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pEngineUniform->SetMainShadowOrtho(indexLevel, left, right, bottom, top, zNear, zFar);
}

void CRenderSystem::SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pEngineUniform->SetMainShadowLookat(indexLevel, eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSystem::Update(CTaskGraph& taskGraph, CCamera* pCamera) const
{
	pCamera->Update(taskGraph);
}

void CRenderSystem::RenderDefault(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent) const
{
	const CGfxSemaphore* pWaitSemaphore = nullptr;

	GfxRenderer()->AcquireNextFrame();
	{
		pWaitSemaphore = pCamera->RenderDefault(taskGraph, GfxRenderer()->GetSwapChain()->GetAcquireSemaphore(), bPresent);
	}
	GfxRenderer()->Present(pWaitSemaphore);
}

void CRenderSystem::RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bShadow, bool bPresent) const
{
	const CGfxSemaphore* pWaitSemaphore = nullptr;

	GfxRenderer()->AcquireNextFrame();
	{
		pWaitSemaphore = pCamera->RenderForwardLighting(taskGraph, GfxRenderer()->GetSwapChain()->GetAcquireSemaphore(), bShadow, bPresent);
	}
	GfxRenderer()->Present(pWaitSemaphore);
}
