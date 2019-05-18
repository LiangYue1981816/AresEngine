#include "GfxHeader.h"


CGfxEngine::CGfxEngine(void)
	: m_pUniformEngine(nullptr)
{
	m_pUniformEngine = new CGfxUniformEngine;

	m_ptrDescriptorLayoutEngine = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_ENGINE);
	m_ptrDescriptorLayoutEngine->SetUniformBlockBinding(UNIFORM_ENGINE_NAME, DESCRIPTOR_BIND_ENGINE);
	m_ptrDescriptorLayoutEngine->Create();

	m_ptrDescriptorSetEngine = GfxRenderer()->NewDescriptorSet(m_ptrDescriptorLayoutEngine);
	m_ptrDescriptorSetEngine->SetUniformBuffer(UNIFORM_ENGINE_NAME, m_pUniformEngine->GetUniformBuffer(), 0, m_pUniformEngine->GetUniformBuffer()->GetSize());
	m_ptrDescriptorSetEngine->Update();
}

CGfxEngine::~CGfxEngine(void)
{
	delete m_pUniformEngine;
}

void CGfxEngine::SetTime(float t, float dt)
{
	m_pUniformEngine->SetTime(t, dt);
}

void CGfxEngine::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pUniformEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CGfxEngine::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pUniformEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CGfxEngine::SetShadowRange(float range)
{
	m_pUniformEngine->SetShadowRange(range);
}

void CGfxEngine::SetShadowResolution(float resolution)
{
	m_pUniformEngine->SetShadowResolution(resolution);
}

void CGfxEngine::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pUniformEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CGfxEngine::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pUniformEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CGfxEngine::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pUniformEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CGfxEngine::SetMainPointLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetPointLightColor(red, green, blue);
}

void CGfxEngine::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pUniformEngine->SetPointLightPosition(posx, posy, posz, radius);
}

void CGfxEngine::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pUniformEngine->SetPointLightAttenuation(linear, square, constant);
}

void CGfxEngine::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetDirectLightColor(red, green, blue);
}

void CGfxEngine::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pUniformEngine->SetDirectLightDirection(dirx, diry, dirz);
}

void CGfxEngine::SetFogColor(float red, float green, float blue)
{
	m_pUniformEngine->SetFogColor(red, green, blue);
}

void CGfxEngine::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pUniformEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CGfxEngine::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pUniformEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}
