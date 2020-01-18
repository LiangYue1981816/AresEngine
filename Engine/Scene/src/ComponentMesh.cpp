#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
	, m_bForceUpdateInstanceData{ true }

	, m_indexLOD{ 0 }
	, m_factor{ 0.0f }
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
	, m_bForceUpdateInstanceData{ true }

	, m_indexLOD{ 0 }
	, m_factor{ 0.0f }
{
	m_indexInstance = RenderSystem()->AddInstance();

	for (int index = 0; index < MAX_LOD_COUNT; index++) {
		m_factor[index] = component.m_factor[index];
		m_ptrMaterial[index] = component.m_ptrMaterial[index];
		m_ptrMeshDraw[index] = component.m_ptrMeshDraw[index];
	}
}

CComponentMesh::~CComponentMesh(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

void CComponentMesh::SetScreenFactor(int indexLOD, float factor)
{
	m_factor[indexLOD] = factor;
}

void CComponentMesh::SetMaterial(int indexLOD, const CGfxMaterialPtr ptrMaterial)
{
	m_ptrMaterial[indexLOD] = ptrMaterial;
}

void CComponentMesh::SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias)
{
	if (nameAlias == INVALID_HASHNAME) {
		m_ptrMeshDraw[indexLOD] = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
	else {
		m_ptrMeshDraw[indexLOD] = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
}

void CComponentMesh::SetMask(int indexLOD, uint32_t mask)
{
	if (m_ptrMeshDraw[indexLOD]) {
		m_ptrMeshDraw[indexLOD]->SetMask(mask);
	}
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;
	/*
	if (m_ptrMeshDraw) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (m_bUpdateInstanceData[indexFrame] == false || m_pParentNode->UpdateTransform()) {
				m_bUpdateInstanceData[indexFrame] = true;
				m_bNeedUpdateInstanceData[indexFrame] = true;
				m_instanceData[indexFrame].transformMatrix = m_pParentNode->GetWorldTransform();
				m_instanceData[indexFrame].center = m_instanceData[indexFrame].transformMatrix * glm::vec4(m_ptrMeshDraw->GetAABB().center, 1.0f);
			}
		}
	}
	*/
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;
	/*
	if (m_ptrMeshDraw) {
		if (m_ptrMeshDraw->GetMask() & mask) {
			if (m_pParentNode && m_pParentNode->IsActive()) {
				if (m_bUpdateInstanceData[indexFrame]) {
					if (pCamera->IsVisible(m_ptrMeshDraw->GetAABB() * m_instanceData[indexFrame].transformMatrix)) {
						if (m_bNeedUpdateInstanceData[indexFrame]) {
							m_bNeedUpdateInstanceData[indexFrame] = false;
							RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
						}

						pRenderQueue->Add(m_ptrMaterial, m_ptrMeshDraw, m_indexInstance, indexThread);
					}
				}
			}
		}
	}
	*/
}

int CComponentMesh::ComputeLOD(void) const
{
	return 0;
}
