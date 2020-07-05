#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstance{ false }

	, m_indexLOD(-1)
	, m_distance2{ 0.0f }
	, m_screenSize2{ 0.0f }
	, m_screenSizeFactor{ 0.0f }

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstance{ false }

	, m_indexLOD(-1)
	, m_distance2{ 0.0f }
	, m_screenSize2{ 0.0f }
	, m_screenSizeFactor{ 0.0f }

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	for (int indexLOD = 0; indexLOD < MAX_LOD_COUNT; indexLOD++) {
		m_ptrMaterial[indexLOD] = component.m_ptrMaterial[indexLOD];
		m_ptrMeshDraw[indexLOD] = component.m_ptrMeshDraw[indexLOD];
		m_screenSizeFactor[indexLOD] = component.m_screenSizeFactor[indexLOD];
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
		m_screenSizeFactor[indexLOD] = factor;
	}
}

void CComponentMesh::SetMaterial(int indexLOD, const CGfxMaterialPtr ptrMaterial)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		m_ptrMaterial[indexLOD] = ptrMaterial;
	}
}

void CComponentMesh::SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t nameAlias)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		if (nameAlias == INVALID_HASHNAME) {
			m_ptrMeshDraw[indexLOD] = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw);
		}
		else {
			m_ptrMeshDraw[indexLOD] = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw);
		}
	}
}

void CComponentMesh::SetMask(int indexLOD, uint32_t mask)
{
	if (indexLOD >= 0 && indexLOD < MAX_LOD_COUNT) {
		if (m_ptrMeshDraw[indexLOD]) {
			m_ptrMeshDraw[indexLOD]->SetMask(mask);
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

bool CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;
	glm::mat4 transformMatrix = m_pParentNode->GetWorldTransform();

	if (m_instances[indexFrame].transformMatrix != transformMatrix) {
		m_instances[indexFrame].transformMatrix  = transformMatrix;

		for (int indexLOD = MAX_LOD_COUNT - 1; indexLOD >= 0; indexLOD--) {
			if (m_ptrMeshDraw[indexLOD] && m_ptrMaterial[indexLOD]) {
				m_aabb[indexLOD] = m_ptrMeshDraw[indexLOD]->GetAABB() * transformMatrix;
			}
		}

		m_bNeedUpdateInstance[indexFrame] = true;
		return true;
	}
	else {
		return false;
	}
}

bool CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (ComputeLOD(bComputeLOD, pCamera->GetPosition())) {
		if ((m_ptrMeshDraw[m_indexLOD]->GetMask() & mask) == 0) {
			return false;
		}

		if (m_distance2[m_indexLOD] > m_cullDistance2) {
			return false;
		}

		if (m_screenSize2[m_indexLOD] < m_cullScreenSize2) {
			return false;
		}

		if (pCamera->IsVisible(m_aabb[m_indexLOD]) == false) {
			return false;
		}

		if (m_bNeedUpdateInstance[indexFrame]) {
			m_bNeedUpdateInstance[indexFrame] = false;
			m_instances[indexFrame].SetCenter(glm::vec4(m_aabb[m_indexLOD].center, 1.0f));
			RenderSystem()->ModifyInstance(m_indexInstance, m_instances[indexFrame], indexThread);
		}

		pRenderQueue->Add(m_ptrMaterial[m_indexLOD], m_ptrMeshDraw[m_indexLOD], m_indexInstance, indexThread);

		return true;
	}
	else {
		return false;
	}
}

bool CComponentMesh::ComputeLOD(bool bComputeLOD, const glm::vec3& cameraPosition)
{
	if (bComputeLOD) {
		m_indexLOD = -1;

		for (int indexLOD = MAX_LOD_COUNT - 1; indexLOD >= 0; indexLOD--) {
			if (m_ptrMeshDraw[indexLOD] && m_ptrMaterial[indexLOD]) {
				m_distance2[indexLOD] = glm::length2(m_aabb[indexLOD].center - cameraPosition);
				m_screenSize2[indexLOD] = glm::min(glm::length2(m_aabb[indexLOD].size()) / glm::max(1.0f, m_distance2[indexLOD]), 1.0f);

				if (m_screenSizeFactor[indexLOD] >= m_screenSize2[indexLOD]) {
					m_indexLOD = indexLOD;
					return true;
				}
			}
		}

		return false;
	}
	else {
		if (m_indexLOD != -1) {
			m_distance2[m_indexLOD] = glm::length2(m_aabb[m_indexLOD].center - cameraPosition);
			m_screenSize2[m_indexLOD] = glm::min(glm::length2(m_aabb[m_indexLOD].size()) / glm::max(1.0f, m_distance2[m_indexLOD]), 1.0f);
			return true;
		}

		return false;
	}
}
