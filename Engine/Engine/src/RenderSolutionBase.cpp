#include "EngineHeader.h"


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


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_pEngine(nullptr)
	, m_pMainCamera(nullptr)
	, m_pShadowCamera(nullptr)
{
	m_pEngine = new CGfxEngine;
	m_pMainCamera = new CGfxCamera;
	m_pShadowCamera = new CGfxCamera;

	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{
	delete m_pEngine;
	delete m_pMainCamera;
	delete m_pShadowCamera;
}

CGfxCamera* CRenderSolutionBase::GetMainCamera(void) const
{
	return m_pMainCamera;
}

CGfxCamera* CRenderSolutionBase::GetShadowCamera(void) const
{
	return m_pShadowCamera;
}

void CRenderSolutionBase::SetTime(float t, float dt)
{
	m_pEngine->SetTime(t, dt);
}

void CRenderSolutionBase::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolutionBase::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolutionBase::SetShadowRange(float range)
{
	m_pEngine->SetShadowRange(range);
}

void CRenderSolutionBase::SetShadowResolution(float resolution)
{
	m_pEngine->SetShadowResolution(resolution);
}

void CRenderSolutionBase::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CRenderSolutionBase::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSolutionBase::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSolutionBase::SetMainPointLightColor(float red, float green, float blue)
{
	m_pEngine->SetMainPointLightColor(red, green, blue);
}

void CRenderSolutionBase::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pEngine->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSolutionBase::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pEngine->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSolutionBase::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pEngine->SetMainDirectLightColor(red, green, blue);
}

void CRenderSolutionBase::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pEngine->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSolutionBase::SetFogColor(float red, float green, float blue)
{
	m_pEngine->SetFogColor(red, green, blue);
}

void CRenderSolutionBase::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSolutionBase::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}
