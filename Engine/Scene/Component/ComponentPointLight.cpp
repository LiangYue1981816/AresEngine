#include "Engine.h"
#include "ComponentPointLight.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
{
	m_ptrMaterial = Renderer()->LoadMaterial("PointLight.material");
	m_ptrMesh = Renderer()->LoadMesh("PointLight.mesh", INSTANCE_ATTRIBUTE_TRANSFORM);
	m_ptrDrawIndirectBuffer = Renderer()->CreateDrawIndirectBuffer(m_ptrMesh, 0, 0, m_ptrMesh->GetIndexCount());
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight &component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMesh = component.m_ptrMesh;
	m_ptrDrawIndirectBuffer = component.m_ptrDrawIndirectBuffer;
}

CComponentPointLight::~CComponentPointLight(void)
{

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

}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(GetWorldAABB())) {
		const glm::mat4 &mtxTransform = m_pParentNode->GetWorldTransform();
//		pCamera->AddMesh(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, (const uint8_t *)&mtxTransform, sizeof(mtxTransform));
		pCamera->AddMeshIndirect(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, m_ptrDrawIndirectBuffer, (const uint8_t *)&mtxTransform, sizeof(mtxTransform));
	}
}
