#include "SceneHeader.h"


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
	SetMesh(component.m_ptrMesh, component.m_indexDraw);
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentMesh::SetMesh(const CGfxMeshPtr &ptrMesh, int indexDraw)
{
	m_ptrMesh = ptrMesh;
	m_indexDraw = indexDraw;
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	return m_pParentNode && m_ptrMesh.IsValid() ? m_ptrMesh->GetLocalAABB(m_indexDraw) * m_pParentNode->GetWorldTransform() : glm::aabb();
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	if (m_pParentNode && m_pParentNode->IsActive()) {
		m_instanceData.transformMatrix = m_pParentNode->GetWorldTransform();
	}
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (m_pParentNode && m_pParentNode->IsActive()) {
		if (pCamera->IsVisible(GetWorldAABB())) {
			pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, m_indexDraw, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
		}
	}
}
