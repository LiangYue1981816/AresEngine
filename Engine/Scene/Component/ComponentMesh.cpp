#include "Engine.h"
#include "ComponentMesh.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMesh = component.m_ptrMesh;

	m_instanceData.transformMatrix = glm::mat4();
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentMesh::SetMesh(const CGfxMeshPtr &ptrMesh)
{
	m_ptrMesh = ptrMesh;
}

glm::aabb CComponentMesh::GetLocalAABB(void)
{
	return m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB() : glm::aabb();
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	return m_pParentNode && m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB() * m_pParentNode->GetWorldTransform() : glm::aabb();
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	m_instanceData.transformMatrix = m_pParentNode->GetWorldTransform();
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(GetWorldAABB())) {
		pCamera->AddMesh(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
	}
}
