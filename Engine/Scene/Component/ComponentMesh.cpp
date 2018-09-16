#include "Engine.h"
#include "ComponentMesh.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_pMesh(NULL)
	, m_pMaterial(NULL)
{

}

CComponentMesh::CComponentMesh(const CComponentMesh &component)
	: CComponent(component.m_name)
	, m_pMesh(NULL)
	, m_pMaterial(NULL)
{
	SetMesh(component.m_pMesh);
	SetMaterial(component.m_pMaterial);
}

CComponentMesh::~CComponentMesh(void)
{
	SetMesh(NULL);
	SetMaterial(NULL);
}

void CComponentMesh::SetMesh(CGfxMesh *pMesh)
{
	if (m_pMesh) {
		m_pMesh->Release();
	}

	m_pMesh = pMesh;

	if (m_pMesh) {
		m_pMesh->Retain();
	}
}

void CComponentMesh::SetMaterial(CGfxMaterial *pMaterial)
{
	if (m_pMaterial) {
		m_pMaterial->Release();
	}

	m_pMaterial = pMaterial;

	if (m_pMaterial) {
		m_pMaterial->Retain();
	}
}

glm::aabb CComponentMesh::GetAABB(void) const
{
	return m_pParentNode && m_pMesh ? m_pMesh->GetAABB() * m_pParentNode->GetWorldTransform() : glm::aabb();
}

CGfxMesh* CComponentMesh::GetMesh(void) const
{
	return m_pMesh;
}

CGfxMaterial* CComponentMesh::GetMaterial(void) const
{
	return m_pMaterial;
}

void CComponentMesh::Update(float deltaTime)
{

}

void CComponentMesh::UpdateCamera(CGfxCamera *pCamera, int indexThread)
{
	if (pCamera->IsVisible(GetAABB())) {
		pCamera->AddQueue(m_pMaterial, m_pMesh, m_pParentNode->GetWorldTransform(), indexThread);
	}
}
