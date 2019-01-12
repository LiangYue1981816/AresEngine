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
	char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "%8.8X_%8.8X", ptrMesh->GetName(), indexDraw);
	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValue(szName), ptrMesh, indexDraw, instanceFormat, instanceBinding);
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	if (m_ptrMeshDraw.IsValid() && m_pParentNode) {
		return m_ptrMeshDraw->GetLocalAABB() * m_pParentNode->GetWorldTransform();
	}
	else {
		return glm::aabb();
	}
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
