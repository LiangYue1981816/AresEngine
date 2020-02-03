#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_indexLOD(-1)

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_indexLOD(-1)

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	for (int index = 0; index < MAX_LOD_COUNT; index++) {
		m_LODMeshDraws[index].screenSizeFactor = component.m_LODMeshDraws[index].screenSizeFactor;
		m_LODMeshDraws[index].ptrMaterial = component.m_LODMeshDraws[index].ptrMaterial;
		m_LODMeshDraws[index].ptrMeshDraw = component.m_LODMeshDraws[index].ptrMeshDraw;
	}

	m_cullDistance2 = component.m_cullDistance2;
	m_cullScreenSize2 = component.m_cullScreenSize2;
}

CComponentMesh::~CComponentMesh(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

void CComponentMesh::SetScreenFactor(int indexLOD, float factor)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		m_LODMeshDraws[indexLOD].screenSizeFactor = factor;
	}
}

void CComponentMesh::SetMaterial(int indexLOD, const CGfxMaterialPtr ptrMaterial)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		m_LODMeshDraws[indexLOD].ptrMaterial = ptrMaterial;
	}
}

void CComponentMesh::SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t nameAlias)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		if (nameAlias == INVALID_HASHNAME) {
			m_LODMeshDraws[indexLOD].ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw);
		}
		else {
			m_LODMeshDraws[indexLOD].ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw);
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

void CComponentMesh::SetCullDistance(float distance)
{
	m_cullDistance2 = distance * distance;
}

void CComponentMesh::SetCullScreenSize(float screenSize)
{
	m_cullScreenSize2 = screenSize * screenSize;
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;

	if (m_instanceData[indexFrame].transformMatrix != m_pParentNode->GetWorldTransform()) {
		m_instanceData[indexFrame].SetTransform(m_pParentNode->GetWorldTransform());
		m_bNeedUpdateInstanceData[indexFrame] = true;

		for (int index = MAX_LOD_COUNT - 1; index >= 0; index--) {
			if (m_LODMeshDraws[index].ptrMeshDraw && m_LODMeshDraws[index].ptrMaterial) {
				m_LODMeshDraws[index].aabb = m_LODMeshDraws[index].ptrMeshDraw->GetAABB() * m_instanceData[indexFrame].transformMatrix;
			}
		}
	}
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (ComputeLOD(bComputeLOD, pCamera->GetPosition())) {
		if ((m_LODMeshDraws[m_indexLOD].ptrMeshDraw->GetMask() & mask) == 0) {
			return;
		}

		if (m_LODMeshDraws[m_indexLOD].distance2 > m_cullDistance2) {
			return;
		}

		if (m_LODMeshDraws[m_indexLOD].screenSize2 < m_cullScreenSize2) {
			return;
		}

		if (pCamera->IsVisible(m_LODMeshDraws[m_indexLOD].aabb) == false) {
			return;
		}

		if (m_bNeedUpdateInstanceData[indexFrame]) {
			m_bNeedUpdateInstanceData[indexFrame] = false;
			m_instanceData[indexFrame].SetCenter(glm::vec4(m_LODMeshDraws[m_indexLOD].aabb.center, 1.0f));
			RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
		}

		pRenderQueue->Add(m_LODMeshDraws[m_indexLOD].ptrMaterial, m_LODMeshDraws[m_indexLOD].ptrMeshDraw, m_indexInstance, indexThread);
	}
}

bool CComponentMesh::ComputeLOD(bool bComputeLOD, const glm::vec3& cameraPosition)
{
	if (bComputeLOD) {
		m_indexLOD = -1;

		for (int index = MAX_LOD_COUNT - 1; index >= 0; index--) {
			if (m_LODMeshDraws[index].ptrMeshDraw && m_LODMeshDraws[index].ptrMaterial) {
				m_LODMeshDraws[index].distance2 = glm::length2(m_LODMeshDraws[index].aabb.center - cameraPosition);
				m_LODMeshDraws[index].screenSize2 = glm::min(glm::length2(m_LODMeshDraws[index].aabb.size()) / glm::max(1.0f, m_LODMeshDraws[index].distance2), 1.0f);

				if (m_LODMeshDraws[index].screenSizeFactor >= m_LODMeshDraws[index].screenSize2) {
					m_indexLOD = index;
					return true;
				}
			}
		}

		return false;
	}
	else {
		if (m_indexLOD != -1) {
			m_LODMeshDraws[m_indexLOD].distance2 = glm::length2(m_LODMeshDraws[m_indexLOD].aabb.center - cameraPosition);
			m_LODMeshDraws[m_indexLOD].screenSize2 = glm::min(glm::length2(m_LODMeshDraws[m_indexLOD].aabb.size()) / glm::max(1.0f, m_LODMeshDraws[m_indexLOD].distance2), 1.0f);
			return true;
		}

		return false;
	}
}
