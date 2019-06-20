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
const uint32_t FORWARD_LIGHTING_PASS_NAME = HashValue("PBRLighting");


CRenderSolution::CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)

	, m_pMainCamera(nullptr)
	, m_pShadowCamera(nullptr)

	, m_pMainQueue(nullptr)
	, m_pLightQueue(nullptr)
	, m_pShadowQueue(nullptr)

	, m_pUniformEngine(nullptr)
	, m_pUniformMainCamera(nullptr)
	, m_pUniformShadowCamera(nullptr)
{
	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	m_pMainCamera = new CGfxCamera;
	m_pShadowCamera = new CGfxCamera;

	m_pMainQueue = new CGfxRenderQueue;
	m_pLightQueue = new CGfxRenderQueue;
	m_pShadowQueue = new CGfxRenderQueue;

	m_pUniformEngine = new CGfxUniformEngine;
	m_pUniformMainCamera = new CGfxUniformCamera;
	m_pUniformShadowCamera = new CGfxUniformCamera;
}

CRenderSolution::~CRenderSolution(void)
{
	delete m_pMainCamera;
	delete m_pShadowCamera;

	delete m_pMainQueue;
	delete m_pLightQueue;
	delete m_pShadowQueue;

	delete m_pUniformEngine;
	delete m_pUniformMainCamera;
	delete m_pUniformShadowCamera;

	delete m_pRenderer;
}

CGfxCamera* CRenderSolution::GetMainCamera(void) const
{
	return m_pMainCamera;
}

CGfxCamera* CRenderSolution::GetShadowCamera(void) const
{
	return m_pShadowCamera;
}

CGfxRenderQueue* CRenderSolution::GetMainQueue(void) const
{
	return m_pMainQueue;
}

CGfxRenderQueue* CRenderSolution::GetLightQueue(void) const
{
	return m_pLightQueue;
}

CGfxRenderQueue* CRenderSolution::GetShadowQueue(void) const
{
	return m_pShadowQueue;
}

CGfxUniformBufferPtr CRenderSolution::GetEngineUniformBuffer(void) const
{
	return m_pUniformEngine->GetUniformBuffer();
}

CGfxUniformBufferPtr CRenderSolution::GetMainCameraUniformBuffer(void) const
{
	return m_pUniformMainCamera->GetUniformBuffer();
}

CGfxUniformBufferPtr CRenderSolution::GetShadowCameraUniformBuffer(void) const
{
	return m_pUniformShadowCamera->GetUniformBuffer();
}

void CRenderSolution::SetRenderSolution(RenderSolution solution, int samples)
{

}

void CRenderSolution::SetTime(float t, float dt)
{
	m_pUniformEngine->SetTime(t, dt);
}

void CRenderSolution::SetCameraScissor(float x, float y, float width, float height)
{
	m_pMainCamera->SetScissor(x, y, width, height);
}

void CRenderSolution::SetCameraViewport(float x, float y, float width, float height)
{
	m_pMainCamera->SetViewport(x, y, width, height);
	m_pUniformMainCamera->SetScreen(width, height);
}

void CRenderSolution::SetCameraPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pMainCamera->SetPerspective(fovy, aspect, zNear, zFar);
	m_pUniformMainCamera->SetPerspective(fovy, aspect, zNear, zFar);
}

void CRenderSolution::SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pMainCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pUniformMainCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolution::SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pMainCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pUniformMainCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolution::SetShadowViewport(float x, float y, float width, float height)
{
	m_pShadowCamera->SetViewport(x, y, width, height);
	m_pUniformShadowCamera->SetScreen(width, height);
}

void CRenderSolution::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pShadowCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pUniformShadowCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pUniformEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolution::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pShadowCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pUniformShadowCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pUniformEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolution::SetShadowRange(float range)
{
	m_pUniformEngine->SetShadowRange(range);
}

void CRenderSolution::SetShadowResolution(float resolution)
{
	m_pUniformEngine->SetShadowResolution(resolution);
}

void CRenderSolution::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pUniformEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CRenderSolution::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pUniformEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSolution::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pUniformEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSolution::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_pUniformEngine->SetAmbientLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetMainPointLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetMainPointLightColor(red, green, blue);
}

void CRenderSolution::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pUniformEngine->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSolution::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pUniformEngine->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSolution::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetMainDirectLightColor(red, green, blue);
}

void CRenderSolution::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pUniformEngine->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetMainFogColor(float red, float green, float blue)
{
	m_pUniformEngine->SetMainFogColor(red, green, blue);
}

void CRenderSolution::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pUniformEngine->SetMainFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSolution::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pUniformEngine->SetMainFogDistanceDensity(startDistance, endDistance, density);
}

void CRenderSolution::UpdateCamera(int indexQueue)
{

}

void CRenderSolution::Render(int indexQueue)
{
	m_pUniformEngine->Apply();
	m_pUniformMainCamera->Apply();
}
