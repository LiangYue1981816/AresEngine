#include "Engine.h"
#include "ComponentPointLight.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
{
	SetMaterial(Renderer()->NewMaterial("PointLight.material"));
	SetMesh(Renderer()->NewMesh("PointLight.mesh", INSTANCE_ATTRIBUTE_TRANSFORM | INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR | INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION));
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight &component)
	: CComponent(component)
{
	SetMaterial(component.m_ptrMaterial);
	SetMesh(component.m_ptrMesh);

	SetColor(component.m_instanceData.color.r, component.m_instanceData.color.g, component.m_instanceData.color.b);
	SetAttenuation(component.m_instanceData.attenuation.x, component.m_instanceData.attenuation.y, component.m_instanceData.attenuation.z);
}

CComponentPointLight::~CComponentPointLight(void)
{

}

void CComponentPointLight::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentPointLight::SetMesh(const CGfxMeshPtr &ptrMesh)
{
	m_ptrMesh = ptrMesh;
	m_localAABB = ptrMesh.IsValid() ? ptrMesh->GetLocalAABB() : glm::aabb();
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
	return m_localAABB;
}

glm::aabb CComponentPointLight::GetWorldAABB(void)
{
	return m_worldAABB;
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	m_worldAABB = m_pParentNode ? m_localAABB * m_pParentNode->GetWorldTransform() : glm::aabb();
	m_instanceData.transformMatrix = m_pParentNode ? m_pParentNode->GetWorldTransform() : glm::mat4();
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(m_worldAABB)) {
		pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, -1, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
	}
}
