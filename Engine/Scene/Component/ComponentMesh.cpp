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
	SetMaterial(component.m_ptrMaterial);
	SetMesh(component.m_ptrMesh);
	SetIndexDraw(component.m_indexDraw);
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
	m_localAABB = ptrMesh.IsValid() ? ptrMesh->GetLocalAABB() : glm::aabb();
}

void CComponentMesh::SetIndexDraw(int indexDraw)
{
	m_indexDraw = indexDraw;
}

glm::aabb CComponentMesh::GetLocalAABB(void)
{
	return m_localAABB;
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	return m_worldAABB;
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	m_worldAABB = m_pParentNode ? m_localAABB * m_pParentNode->GetWorldTransform() : glm::aabb();
	m_instanceData.transformMatrix = m_pParentNode ? m_pParentNode->GetWorldTransform() : glm::mat4();
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(m_worldAABB)) {
		pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, -1, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
	}
}
