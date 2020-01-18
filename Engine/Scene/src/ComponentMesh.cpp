#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
	, m_bForceUpdateInstanceData{ true, true }

	, m_LODIndex{ 0 }
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
	, m_bForceUpdateInstanceData{ true, true }

	, m_LODIndex{ 0 }
{
	m_indexInstance = RenderSystem()->AddInstance();

	for (int index = 0; index < MAX_LOD_COUNT; index++) {
		m_LODMeshDraws[index].factor = component.m_LODMeshDraws[index].factor;
		m_LODMeshDraws[index].ptrMaterial = component.m_LODMeshDraws[index].ptrMaterial;
		m_LODMeshDraws[index].ptrMeshDraw = component.m_LODMeshDraws[index].ptrMeshDraw;
	}
}

CComponentMesh::~CComponentMesh(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

void CComponentMesh::SetScreenFactor(int indexLOD, float factor)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		m_LODMeshDraws[indexLOD].factor = factor;
	}
}

void CComponentMesh::SetMaterial(int indexLOD, const CGfxMaterialPtr ptrMaterial)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		m_LODMeshDraws[indexLOD].ptrMaterial = ptrMaterial;
	}
}

void CComponentMesh::SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		if (nameAlias == INVALID_HASHNAME) {
			m_LODMeshDraws[indexLOD].ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw, instanceFormat, instanceBinding);
		}
		else {
			m_LODMeshDraws[indexLOD].ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw, instanceFormat, instanceBinding);
		}
	}
}

void CComponentMesh::SetMask(int indexLOD, uint32_t mask)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		if (m_LODMeshDraws[indexLOD].ptrMeshDraw) {
			m_LODMeshDraws[indexLOD].ptrMeshDraw->SetMask(mask);
		}
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
