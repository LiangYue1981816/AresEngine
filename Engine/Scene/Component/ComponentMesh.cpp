#include "Engine.h"
#include "ComponentMesh.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component)
{
	SetMaterial(component.m_ptrMaterial);
	SetMesh(component.m_ptrMesh);
	SetDrawIndirectBuffer(component.m_ptrDrawIndirectBuffer);
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

void CComponentMesh::SetDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer)
{
	m_ptrDrawIndirectBuffer = ptrDrawIndirectBuffer;
}

const CGfxMaterialPtr& CComponentMesh::GetMaterial(void) const
{
	return m_ptrMaterial;
}

const CGfxMeshPtr& CComponentMesh::GetMesh(void) const
{
	return m_ptrMesh;
}

const CGfxDrawIndirectBufferPtr& CComponentMesh::GetDrawIndirectBuffer(void) const
{
	return m_ptrDrawIndirectBuffer;
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

void CComponentMesh::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{
	if (pCamera->IsVisible(GetWorldAABB())) {
		const glm::mat4 &mtxTransform = m_pParentNode->GetWorldTransform();
//		pCamera->AddMesh(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, (const uint8_t *)&mtxTransform, sizeof(mtxTransform));
		pCamera->AddMeshIndirect(indexThread, indexQueue, m_ptrMaterial, m_ptrMesh, m_ptrDrawIndirectBuffer, (const uint8_t *)&mtxTransform, sizeof(mtxTransform));
	}
}
