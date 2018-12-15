#include "SceneHeader.h"
#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr &ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentMesh::SetMeshDraw(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(m_name, ptrMesh, indexDraw, instanceFormat, instanceBinding);
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	return m_pParentNode && m_ptrMeshDraw.IsValid() ? m_ptrMeshDraw->GetLocalAABB() * m_pParentNode->GetWorldTransform() : glm::aabb();
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
			pCamera->Add(indexThread, indexQueue, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t *)&m_instanceData, sizeof(m_instanceData));
		}
	}
}
