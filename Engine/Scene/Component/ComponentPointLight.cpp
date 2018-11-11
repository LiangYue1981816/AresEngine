#include "Engine.h"
#include "ComponentPointLight.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
{
	m_ptrMaterial = Renderer()->NewMaterial("PointLight.material");
	m_ptrMesh = Renderer()->NewMesh("PointLight.mesh", INSTANCE_ATTRIBUTE_TRANSFORM | INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR | INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION);
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight &component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMesh = component.m_ptrMesh;

	m_instanceData.transformMatrix = glm::mat4();
	m_instanceData.color = component.m_instanceData.color;
	m_instanceData.attenuation = component.m_instanceData.attenuation;
}

CComponentPointLight::~CComponentPointLight(void)
{

}

void CComponentPointLight::SetColor(float red, float green, float blue)
{
	m_instanceData.color = glm::vec4(red, green, blue, 0.0f);
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_instanceData.attenuation = glm::vec4(linear, square, constant, 0.0f);
}

glm::aabb CComponentPointLight::GetLocalAABB(void)
{
	return m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB() : glm::aabb();
}

glm::aabb CComponentPointLight::GetWorldAABB(void)
{
	return m_pParentNode && m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB() * m_pParentNode->GetWorldTransform() : glm::aabb();
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	m_instanceData.transformMatrix = m_pParentNode->GetWorldTransform();
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(GetWorldAABB())) {
		pCamera->AddMesh(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
	}
}
