#include "SceneHeader.h"


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
}

void CComponentPointLight::SetColor(float red, float green, float blue)
{
	m_instanceData.color = glm::vec4(red, green, blue, 0.0f);
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_instanceData.attenuation = glm::vec4(linear, square, constant, 0.0f);
}

glm::aabb CComponentPointLight::GetWorldAABB(void)
{
	return m_pParentNode && m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB(0) * m_pParentNode->GetWorldTransform() : glm::aabb();
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	if (m_pParentNode && m_pParentNode->IsActive()) {
		m_instanceData.transformMatrix = m_pParentNode->GetWorldTransform();
	}
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (m_pParentNode && m_pParentNode->IsActive()) {
		if (pCamera->IsVisible(GetWorldAABB())) {
			pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, 0, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
		}
	}
}
