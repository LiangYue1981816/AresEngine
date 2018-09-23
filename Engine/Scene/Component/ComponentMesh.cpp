#include "Engine.h"
#include "ComponentMesh.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component)
{
	SetMesh(component.m_ptrMesh);
	SetMaterial(component.m_ptrMaterial);
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetMesh(const CGfxMeshPtr &ptrMesh)
{
	m_ptrMesh = ptrMesh;
}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

const CGfxMeshPtr& CComponentMesh::GetMesh(void) const
{
	return m_ptrMesh;
}

const CGfxMaterialPtr& CComponentMesh::GetMaterial(void) const
{
	return m_ptrMaterial;
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

}

void CComponentMesh::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread)
{
	if (pCamera->IsVisible(GetWorldAABB())) {
		pCamera->AddQueue(m_ptrMaterial, m_ptrMesh, m_pParentNode->GetWorldTransform(), indexThread);
	}
}
