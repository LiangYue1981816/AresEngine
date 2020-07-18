#include "EngineHeader.h"
#include "RenderHeader.h"


/*
CUniformEngine::CUniformEngine(void)
	: m_bDirty(false)
	, m_offset(0)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * GfxRenderer()->AlignUniformBufferOffset(sizeof(m_params)));
}

CUniformEngine::~CUniformEngine(void)
{

}

const CUniformEngine::Params& CUniformEngine::GetParams(void) const
{
	return m_params;
}

uint32_t CUniformEngine::GetUniformBufferSize(void) const
{
	return sizeof(m_params);
}

uint32_t CUniformEngine::GetUniformBufferOffset(void) const
{
	return m_offset;
}

CGfxUniformBufferPtr CUniformEngine::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CUniformEngine::SetTime(float t, float dt)
{
	m_bDirty = true;
	m_params.time = glm::vec4(t / 20.0f, t * 1.0f, t * 2.0f, t * 3.0f);
	m_params.sinTime = glm::vec4(sinf(t / 8.0f), sinf(t / 4.0f), sinf(t / 2.0f), t);
	m_params.cosTime = glm::vec4(cosf(t / 8.0f), cosf(t / 4.0f), cosf(t / 2.0f), t);
	m_params.deltaTime = glm::vec4(dt, 1.0f / dt, 1.0f, 1.0f);
}

void CUniformEngine::SetEnvLightFactor(float factor)
{
	m_bDirty = true;
	m_params.lightFactor.w = factor;
}

void CUniformEngine::SetAmbientLightFactor(float factor)
{
	m_bDirty = true;
	m_params.lightFactor.x = factor;
}

void CUniformEngine::SetMainPointLightFactor(float factor)
{
	m_bDirty = true;
	m_params.lightFactor.y = factor;
}

void CUniformEngine::SetMainDirectLightFactor(float factor)
{
	m_bDirty = true;
	m_params.lightFactor.z = factor;
}

void CUniformEngine::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_bDirty = true;
	m_params.ambientSHLight0 = glm::vec4(shRed[0], shRed[1], shRed[2], 0.0);
	m_params.ambientSHLight1 = glm::vec4(shRed[3], shRed[4], shRed[5], 0.0);
	m_params.ambientSHLight2 = glm::vec4(shRed[6], shRed[7], shRed[8], 0.0);
	m_params.ambientSHLight3 = glm::vec4(shGreen[0], shGreen[1], shGreen[2], 0.0);
	m_params.ambientSHLight4 = glm::vec4(shGreen[3], shGreen[4], shGreen[5], 0.0);
	m_params.ambientSHLight5 = glm::vec4(shGreen[6], shGreen[7], shGreen[8], 0.0);
	m_params.ambientSHLight6 = glm::vec4(shBlue[0], shBlue[1], shBlue[2], 0.0);
	m_params.ambientSHLight7 = glm::vec4(shBlue[3], shBlue[4], shBlue[5], 0.0);
	m_params.ambientSHLight8 = glm::vec4(shBlue[6], shBlue[7], shBlue[8], 0.0);
}

void CUniformEngine::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_bDirty = true;
	m_params.ambientSHLightRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(axisx, axisy, axisz));
}

void CUniformEngine::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.ambientSHLightRotationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(dirx, diry, dirz)), glm::vec3(0.0f, 1.0f, 0.0f));
}

void CUniformEngine::SetMainPointLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainPointLightColor = glm::vec4(red, green, blue, 0.0f);
}

void CUniformEngine::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_bDirty = true;
	m_params.mainPointLightPosition = glm::vec4(posx, posy, posz, radius);
}

void CUniformEngine::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_bDirty = true;
	m_params.mainPointLightAttenuation = glm::vec4(linear, square, constant, 0.0f);
}

void CUniformEngine::SetMainDirectLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainDirectLightColor = glm::vec4(red, green, blue, 0.0);
}

void CUniformEngine::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.mainDirectLightDirection = glm::vec4(glm::normalize(glm::vec3(-dirx, -diry, -dirz)), 0.0f);
}

void CUniformEngine::SetMainFogColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainFogColor = glm::vec4(red, green, blue, 0.0f);
}

void CUniformEngine::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_bDirty = true;
	m_params.mainFogHeightParams = glm::vec4(startHeight, endHeight, density, 0.0f);
}

void CUniformEngine::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_bDirty = true;
	m_params.mainFogDistanceParams = glm::vec4(startDistance, endDistance, density, 0.0f);
}

void CUniformEngine::SetMainShadowLevelFactor(float factor0, float factor1, float factor2, float factor3)
{
	m_bDirty = true;
	m_params.mainShadowLevelFactor = glm::vec4(factor0, factor1, factor2, factor3);
}

void CUniformEngine::SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.mainShadowDepth[indexLevel] = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
		m_params.mainShadowProjectionMatrix[indexLevel] = GfxRenderer()->GetBaseMatrix() * glm::ortho(left, right, bottom, top, zNear, zFar);
		m_params.mainShadowProjectionViewMatrix[indexLevel] = m_params.mainShadowProjectionMatrix[indexLevel] * m_params.mainShadowViewMatrix[indexLevel];
	}
}

void CUniformEngine::SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.mainShadowViewMatrix[indexLevel] = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
		m_params.mainShadowViewInverseMatrix[indexLevel] = glm::inverse(m_params.mainShadowViewMatrix[indexLevel]);
		m_params.mainShadowProjectionViewMatrix[indexLevel] = m_params.mainShadowProjectionMatrix[indexLevel] * m_params.mainShadowViewMatrix[indexLevel];
	}
}

void CUniformEngine::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_offset = GfxRenderer()->GetSwapChain()->GetFrameIndex() * GfxRenderer()->AlignUniformBufferOffset(sizeof(m_params));
		m_ptrUniformBuffer->BufferData(m_offset, sizeof(m_params), &m_params);
	}
}
*/