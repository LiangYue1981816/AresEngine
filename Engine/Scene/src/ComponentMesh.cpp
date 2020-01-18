#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_cullDistance(FLT_MAX)
	, m_cullScreenSize(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

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

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	int indexLOD = 0;
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	const glm::vec3& cameraPosition = pCamera->GetPosition();
	const glm::mat4& projectionMatrix = pCamera->GetProjectionMatrix();
	const glm::mat4& transformMatrix = m_instanceData[indexFrame].transformMatrix;

	if (ComputeLOD(indexLOD, cameraPosition, projectionMatrix, transformMatrix)) {
		const LODMeshDraw& mesh = m_LODMeshDraws[indexLOD];

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

bool CComponentMesh::ComputeLOD(int& indexLOD, const glm::vec3& cameraPosition, const glm::mat4& projectionMatrix, const glm::mat4& transformMatrix)
{
	indexLOD = -1;

	const float multiple = glm::max(0.5f * projectionMatrix[0][0], 0.5f * projectionMatrix[1][1]);
	const float multiple2 = multiple * multiple;

	for (int index = MAX_LOD_COUNT - 1; index >= 0; index--) {
		if (m_LODMeshDraws[index].ptrMeshDraw && m_LODMeshDraws[index].ptrMaterial) {
			const glm::aabb aabb = m_LODMeshDraws[index].ptrMeshDraw->GetAABB() * transformMatrix;
			const float length2 = glm::length2(aabb.center - cameraPosition);
			const float screenSize2 = glm::min(glm::length2(aabb.maxVertex - aabb.minVertex) * multiple2 / glm::max(1.0f, length2), 1.0f);

			if (m_LODMeshDraws[index].factor >= screenSize2) {
				m_LODMeshDraws[index].aabb = aabb;
				m_LODMeshDraws[index].length2 = length2;
				m_LODMeshDraws[index].screenSize2 = screenSize2;
				indexLOD = index;
				break;
			}
		}
	}

	return indexLOD != -1;
}
