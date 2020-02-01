#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_indexLOD(0)

	, m_cullDistance(FLT_MAX)
	, m_cullScreenSize(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_indexLOD(0)

	, m_cullDistance(FLT_MAX)
	, m_cullScreenSize(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	for (int index = 0; index < MAX_LOD_COUNT; index++) {
		m_LODMeshDraws[index].factor = component.m_LODMeshDraws[index].factor;
		m_LODMeshDraws[index].ptrMaterial = component.m_LODMeshDraws[index].ptrMaterial;
		m_LODMeshDraws[index].ptrMeshDraw = component.m_LODMeshDraws[index].ptrMeshDraw;
	}

	m_cullDistance = component.m_cullDistance;
	m_cullScreenSize = component.m_cullScreenSize;
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
	m_cullDistance = distance;
}

void CComponentMesh::SetCullScreenSize(float screenSize)
{
	m_cullScreenSize = screenSize;
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;

	if (m_instanceData[indexFrame].transformMatrix != m_pParentNode->GetWorldTransform()) {
		m_instanceData[indexFrame].transformMatrix  = m_pParentNode->GetWorldTransform();
		m_bNeedUpdateInstanceData[indexFrame] = true;
	}
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (ComputeLOD(bComputeLOD, pCamera->GetPosition(), m_instanceData[indexFrame].transformMatrix)) {
		const LODMeshDraw& mesh = m_LODMeshDraws[m_indexLOD];

		if (mesh.length2 > m_cullDistance * m_cullDistance) {
			return;
		}

		if (mesh.screenSize2 < m_cullScreenSize * m_cullScreenSize) {
			return;
		}

		if (pCamera->IsVisible(mesh.aabb) == false) {
			return;
		}

		if (m_bNeedUpdateInstanceData[indexFrame]) {
			m_bNeedUpdateInstanceData[indexFrame] = false;
			m_instanceData[indexFrame].center = glm::vec4(mesh.aabb.center, 1.0f);
			RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
		}

		pRenderQueue->Add(mesh.ptrMaterial, mesh.ptrMeshDraw, m_indexInstance, indexThread);
	}
}

bool CComponentMesh::ComputeLOD(bool bComputeLOD, const glm::vec3& cameraPosition, const glm::mat4& transformMatrix)
{
	if (bComputeLOD) {
		m_indexLOD = -1;

		for (int index = MAX_LOD_COUNT - 1; index >= 0; index--) {
			if (m_LODMeshDraws[index].ptrMeshDraw &&
				m_LODMeshDraws[index].ptrMaterial) {
				m_LODMeshDraws[index].aabb = m_LODMeshDraws[index].ptrMeshDraw->GetAABB() * transformMatrix;
				m_LODMeshDraws[index].length2 = glm::length2(m_LODMeshDraws[index].aabb.center - cameraPosition);
				m_LODMeshDraws[index].screenSize2 = glm::min(glm::length2(m_LODMeshDraws[index].aabb.size()) / glm::max(1.0f, m_LODMeshDraws[index].length2), 1.0f);

				if (m_LODMeshDraws[index].factor >= m_LODMeshDraws[index].screenSize2) {
					m_indexLOD = index;
					break;
				}
			}
		}
	}

	return m_indexLOD != -1;
}
