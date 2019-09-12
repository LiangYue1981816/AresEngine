#include "SceneHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;
}

CComponentMesh::~CComponentMesh(void)
{

}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentMesh::SetMeshDraw(const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias)
{
	if (nameAlias == INVALID_HASHNAME) {
		m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
	else {
		m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
}

void CComponentMesh::SetMask(uint32_t mask)
{
	if (m_ptrMeshDraw) {
		m_ptrMeshDraw->SetMask(mask);
	}
}

glm::aabb CComponentMesh::GetWorldAABB(void)
{
	if (m_ptrMeshDraw && m_pParentNode) {
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

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	if (m_ptrMeshDraw->GetMask() & mask) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (pCamera->IsVisible(GetWorldAABB())) {
				pRenderQueue->Add(indexThread, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)& m_instanceData, sizeof(m_instanceData));
			}
		}
	}
}
