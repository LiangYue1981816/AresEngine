#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
{
	m_indexInstance = RenderSystem()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
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

	if (m_instanceData[indexFrame].transformMatrix != m_pParentNode->GetWorldTransform()) {
		m_instanceData[indexFrame].transformMatrix  = m_pParentNode->GetWorldTransform();
		m_bNeedUpdateInstanceData[indexFrame] = true;
	}
}

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	int indexLOD = 0;
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (ComputeLOD(indexLOD, pCamera, m_instanceData[indexFrame].transformMatrix)) {
		const LODMeshDraw& mesh = m_LODMeshDraws[indexLOD];

		if (pCamera->IsVisible(mesh.ptrMeshDraw->GetAABB() * m_instanceData[indexFrame].transformMatrix) == false) {
			return;
		}

		if (m_bNeedUpdateInstanceData[indexFrame]) {
			m_bNeedUpdateInstanceData[indexFrame] = false;
			m_instanceData[indexFrame].center = m_instanceData[indexFrame].transformMatrix * glm::vec4(mesh.ptrMeshDraw->GetAABB().center, 1.0f);
			RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
		}

		pRenderQueue->Add(mesh.ptrMaterial, mesh.ptrMeshDraw, m_indexInstance, indexThread);
	}
}

bool CComponentMesh::ComputeLOD(int& indexLOD, const CGfxCamera* pCameram, const glm::mat4& transformMatrix) const
{
	indexLOD = -1;

	const glm::mat4& viewTransformMatrix = pCameram->GetViewMatrix() * transformMatrix;
	const glm::mat4& projectionMatrix = pCameram->GetProjectionMatrix();
	const float multiple = std::max(0.5f * projectionMatrix[0][0], 0.5f * projectionMatrix[1][1]);
	const float multiple2 = multiple * multiple;

	for (int index = MAX_LOD_COUNT - 1; index >= 0; index--) {
		if (m_LODMeshDraws[index].ptrMeshDraw) {
			const glm::aabb aabb = m_LODMeshDraws[index].ptrMeshDraw->GetAABB() * viewTransformMatrix;
			const float size2 = glm::length2(aabb.maxVertex - aabb.minVertex);
			const float length2 = glm::length2(aabb.center);
			const float screenSize = (multiple2 * size2) / std::max(1.0f, length2);

			if (m_LODMeshDraws[index].factor > screenSize) {
				indexLOD = index;
				break;
			}
		}
	}

	return indexLOD != -1;
}
