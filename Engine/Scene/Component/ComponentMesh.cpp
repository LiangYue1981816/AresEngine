#include "Engine.h"
#include "ComponentMesh.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexDraw(-1)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component)
	, m_indexDraw(-1)
{
	m_indexDraw = component.m_indexDraw;
	m_ptrMesh = component.m_ptrMesh;
	m_ptrMaterial = component.m_ptrMaterial;

	m_instanceData.transformMatrix = glm::mat4();
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetIndexDraw(int indexDraw)
{
	m_indexDraw = indexDraw;
}

void CComponentMesh::SetMesh(const CGfxMeshPtr &ptrMesh)
{
	m_ptrMesh = ptrMesh;
}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
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
		pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, m_indexDraw, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
	}
}
