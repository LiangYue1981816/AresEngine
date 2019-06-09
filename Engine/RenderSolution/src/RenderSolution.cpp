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


CRenderSolution::CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)
	, m_pRenderSolution{ nullptr }
	, m_pCurrentRenderSolution(nullptr)

	, m_pEngine(nullptr)

	, m_pMainCamera(nullptr)
	, m_pShadowCamera(nullptr)

	, m_pMainQueue(nullptr)
	, m_pLightQueue(nullptr)
	, m_pShadowQueue(nullptr)
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

	m_pEngine = new CGfxEngine;

	m_pMainCamera = new CGfxCamera;
	m_pShadowCamera = new CGfxCamera;

	m_pMainQueue = new CGfxRenderQueue;
	m_pLightQueue = new CGfxRenderQueue;
	m_pShadowQueue = new CGfxRenderQueue;

	CRenderSolutionBase::CreateInstance();
	m_pRenderSolution[RENDER_SOLUTION_DEFAULT] = new CRenderSolutionDefault(this);
	m_pRenderSolution[RENDER_SOLUTION_FORWARD] = new CRenderSolutionForward(this);
	m_pRenderSolution[RENDER_SOLUTION_DEFERRED] = new CRenderSolutionDeferred(this);
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_FORWARD] = new CRenderSolutionTiledBaseForward(this);
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED] = new CRenderSolutionTiledBaseDeferred(this);

	m_pCurrentRenderSolution = m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	m_pCurrentRenderSolution->Create();
}

CRenderSolution::~CRenderSolution(void)
{
	delete m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	delete m_pRenderSolution[RENDER_SOLUTION_FORWARD];
	delete m_pRenderSolution[RENDER_SOLUTION_DEFERRED];
	delete m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_FORWARD];
	delete m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED];
	CRenderSolutionBase::DestroyInstance();

	delete m_pEngine;

	delete m_pMainCamera;
	delete m_pShadowCamera;

	delete m_pMainQueue;
	delete m_pLightQueue;
	delete m_pShadowQueue;

	delete m_pRenderer;
}

CGfxEngine* CRenderSolution::GetEngine(void) const
{
	return m_pEngine;
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

void CRenderSolution::SetCurrentRenderSolution(RenderSolution solution, int samples)
{
	m_pCurrentRenderSolution->Destroy();
	m_pCurrentRenderSolution = m_pRenderSolution[solution];
	m_pCurrentRenderSolution->Create(samples);
}

void CRenderSolution::SetTime(float t, float dt)
{
	m_pEngine->SetTime(t, dt);
}

void CRenderSolution::SetCameraScissor(float x, float y, float width, float height)
{
	m_pMainCamera->SetScissor(x, y, width, height);
}

void CRenderSolution::SetCameraViewport(float x, float y, float width, float height)
{
	m_pEngine->SetCameraViewport(x, y, width, height);
	m_pMainCamera->SetViewport(x, y, width, height);
}

void CRenderSolution::SetCameraPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pEngine->SetCameraPerspective(fovy, aspect, zNear, zFar);
	m_pMainCamera->SetPerspective(fovy, aspect, zNear, zFar);
}

void CRenderSolution::SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pEngine->SetCameraOrtho(left, right, bottom, top, zNear, zFar);
	m_pMainCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolution::SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pEngine->SetCameraLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pMainCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolution::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
	m_pShadowCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolution::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pShadowCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolution::SetShadowRange(float range)
{
	m_pEngine->SetShadowRange(range);
}

void CRenderSolution::SetShadowResolution(float resolution)
{
	m_pEngine->SetShadowResolution(resolution);
}

void CRenderSolution::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CRenderSolution::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSolution::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSolution::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_pEngine->SetAmbientLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetMainPointLightColor(float red, float green, float blue)
{
	m_pEngine->SetMainPointLightColor(red, green, blue);
}

void CRenderSolution::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pEngine->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSolution::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pEngine->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSolution::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pEngine->SetMainDirectLightColor(red, green, blue);
}

void CRenderSolution::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pEngine->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSolution::SetFogColor(float red, float green, float blue)
{
	m_pEngine->SetFogColor(red, green, blue);
}

void CRenderSolution::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSolution::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}

void CRenderSolution::UpdateCamera(int indexQueue)
{
	m_pCurrentRenderSolution->UpdateCamera(indexQueue);
}

void CRenderSolution::Render(int indexQueue)
{
	m_pCurrentRenderSolution->Render(indexQueue);
}
